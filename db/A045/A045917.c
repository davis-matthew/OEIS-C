/*
 * A045917  From Goldbach problem: number of decompositions of 2n into
 *          unordered sums of two primes.
 * https://oeis.org/A045917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 3464, u16 n <= 9758, u32 n <= 9758, u64 n <= 9758,
 *            u128 n <= 9758, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of unordered decompositions of 2·n into two primes
 *
 *  For each prime p ≤ n let q = 2n – p.  If q is prime (which forces
 *  q ≥ p) then {p, q} is a Goldbach representation of 2n.  The count
 *  over all such p gives a(n).  The algorithm computes a sieve once up
 *  to the maximum 2n requested and uses a binary search in the prime
 *  list to test membership of q.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *prime_list = NULL;   /* array of primes */
static size_t     prime_cnt  = 0;     /* number of primes stored   */
static idx_t      prime_limit = 0;    /* largest prime computed    */

/* Ensure that we have a list of all primes ≤ limit.  The sieve is
 * performed only once as calls grow larger; for smaller limits the
 * existing list is kept.  The sieve implementation is a straightforward
 * odd‑only Sieve of Eratosthenes; it is fast enough for the required
 * limits (≈ 20000 for n ≤ 10000).  All intermediate values fit in
 * idx_t (uint64_t). */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)            /* already have enough primes */
        return;

    /* Allocate a boolean array; one byte per entry is fine for small limits. */
    bool *is_prime = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime)                     /* fatal error if allocation fails */
        exit(EXIT_FAILURE);

    if (limit >= 2)
        is_prime[2] = true;
    for (idx_t i = 3; i <= limit; i += 2)
        is_prime[i] = true;

    for (idx_t i = 3; i * i <= limit; i += 2)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += 2 * i)
                is_prime[j] = false;

    /* Count primes: from this we know array size. */
    size_t needed = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++needed;

    uint64_t *new_list = (uint64_t*)malloc(needed * sizeof(uint64_t));
    if (!new_list)
        exit(EXIT_FAILURE);

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            new_list[pos++] = i;

    /* Replace old list. */
    free(prime_list);
    prime_list = new_list;
    prime_cnt  = needed;
    prime_limit = limit;
    free(is_prime);
}

/* Binary search in the sorted prime_list array.  Returns true if
 * value is present.  The list is stored as uint64_t but the indices
 * are of type idx_t. */
static bool binary_search_prime(uint64_t value)
{
    size_t low = 0;
    size_t high = prime_cnt;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (prime_list[mid] == value)
            return true;
        if (prime_list[mid] < value)
            low = mid + 1;
        else
            high = mid;
    }
    return false;
}

/* Main sequence function.  The function is fast enough for n up to
 * 10 000 and uses memoised primes, so repeated or out‑of‑order calls
 * are O(1) after the first sieve. */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n < 1, but harmless */
        return 0;

    idx_t two_n = n * 2;
    ensure_primes(two_n);    /* primes up to 2n are needed */

    seq_t count = 0;
    /* Iterate over primes p ≤ n.  For each such p, check if q = 2n – p
     * is prime.  Because p ≤ n, we have q ≥ n ≥ p, so each unordered
     * pair is counted exactly once. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n)          /* only primes ≤ n are relevant */
            break;

        uint64_t q = (uint64_t)two_n - p;
        if (binary_search_prime(q))
            ++count;
    }
    return count;
}
