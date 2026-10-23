/*
 * A104477  Number of successive squarefree intervals between primes.
 * https://oeis.org/A104477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n is even; otherwise let k = (n+1)/2 and a(n) =
 *            prime_pi((k+1)^2) - prime_pi(k^2) - 1, where prime_pi(x) =
 *            number of primes ≤ x
 * Range:     u8 n <= 3512, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static data for incremental sieve */
static uint64_t *prime_list = NULL;   /* primes ≤ current sieve_limit      */
static size_t   prime_count = 0;      /* number of primes in prime_list     */
static uint64_t sieve_limit = 0;      /* current maximum prime considered   */

/* Build a simple Eratosthenes sieve up to 'limit' and store primes.  */
static void build_sieve(uint64_t limit)
{
    if (limit <= sieve_limit) return;

    /* Temporary boolean array for the sieve. */
    unsigned char *is_prime = (unsigned char *)malloc(limit + 1);
    if (!is_prime) return; /* Fallback: leave previous state unchanged. */

    memset(is_prime, 1, limit + 1);
    is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (uint64_t q = p * p; q <= limit; q += p) {
                is_prime[q] = 0;
            }
        }
    }

    /* Count primes to allocate array. */
    size_t new_count = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) ++new_count;
    }

    /* Allocate prime list. */
    uint64_t *new_list = (uint64_t *)malloc(new_count * sizeof(uint64_t));
    if (!new_list) {
        free(is_prime);
        return; /* Allocation failed – keep old state. */
    }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) new_list[idx++] = i;
    }

    free(is_prime);

    /* Replace old data. */
    free(prime_list);
    prime_list = new_list;
    prime_count = new_count;
    sieve_limit = limit;
}

/* Prime counting function using binary search on prime_list. */
static uint64_t prime_pi(uint64_t x)
{
    if (prime_list == NULL || x < prime_list[0]) return 0;
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo; /* Number of primes ≤ x */
}

seq_t a(idx_t n)
{
    /* Even indices are always 0. */
    if ((n & 1ULL) == 0) return 0;

    /* For odd n: k = (n + 1) / 2. */
    uint64_t k = (n + 1ULL) >> 1; /* fast division by 2 */

    /* Bounds for prime counting. */
    uint64_t lower = k * k;
    uint64_t upper = (k + 1ULL) * (k + 1ULL);

    /* Ensure sieve covers 'upper'. */
    if (upper > sieve_limit) build_sieve(upper);

    uint64_t pi_upper = prime_pi(upper);
    uint64_t pi_lower = prime_pi(lower);

    /* Result fits in seq_t for expected n. */
    return (seq_t)(pi_upper - pi_lower - 1ULL);
}
