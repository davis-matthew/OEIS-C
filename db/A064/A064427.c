/*
 * A064427  a(n) = n + (number of primes < n).
 * https://oeis.org/A064427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + π(n−1) (number of primes strictly less than n)
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(1) = 1, a(2) = 2, a(3) = 4, … as defined in OEIS A064427  */

/* --- Includes ------------------------------------------------------- */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --- Static data for the incremental sieve --------------------------- */
static idx_t      prime_mark_limit = 0;   /* largest index currently sieved   */
static bool      *prime_mark       = NULL; /* prime_mark[i] == true ⇔ i is prime */
static uint64_t  *prime_list       = NULL; /* list of primes found so far      */
static size_t    prime_cap   = 0;         /* allocated capacity for prime_list */
static size_t    prime_count = 0;         /* number of primes in prime_list   */

/* --- Helper: add a new prime to prime_list (expanding if needed) ---- */
static void _add_prime(uint64_t p)
{
    if (prime_count == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
        if (!tmp) abort(); /* out of memory */
        prime_list = tmp;
    }
    prime_list[prime_count++] = p;
}

/* --- Helper: extend the sieve to at least 'limit' ------------------- */
static void _ensure_sieve(idx_t limit)
{
    if (limit <= prime_mark_limit)
        return; /* already covered */

    size_t old_limit = prime_mark_limit;
    size_t new_limit = (size_t)limit;

    /* Expand the prime_mark array */
    bool *tmp = (bool*)realloc(prime_mark, (new_limit + 1) * sizeof(bool));
    if (!tmp) abort();
    prime_mark = tmp;

    /* Initialise any newly added entries as prime candidates */
    for (size_t i = old_limit + 1; i <= new_limit; ++i)
        prime_mark[i] = true;
    if (new_limit >= 0) prime_mark[0] = false;
    if (new_limit >= 1) prime_mark[1] = false;

    /* Perform the sieve for the new range only (but using all primes up to sqrt) */
    for (size_t p = 2; p * p <= new_limit; ++p) {
        if (prime_mark[p]) {
            size_t start = (old_limit < p * p) ? p * p : ((old_limit + p) / p) * p;
            for (size_t j = start; j <= new_limit; j += p)
                prime_mark[j] = false;
        }
    }

    /* Record any new primes found in the expanded interval */
    for (size_t i = old_limit + 1; i <= new_limit; ++i) {
        if (prime_mark[i])
            _add_prime((uint64_t)i);
    }

    prime_mark_limit = new_limit;
}

/* --- Helper: count primes ≤ n using binary search on prime_list -- */
static size_t _prime_pi(idx_t n)
{
    if (n < 2)
        return 0;
    size_t left  = 0;
    size_t right = prime_count;
    while (left < right) {
        size_t mid = (left + right) / 2;
        if (prime_list[mid] <= (uint64_t)n)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

/* -------------------------------------------------------------------- */
/* Public sequence function required by the problem description        */
extern seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* undefined for OEIS but safe */

    idx_t target = n - 1;                     /* primes strictly less than n */
    _ensure_sieve(target);
    size_t pi = _prime_pi(target);

    return (seq_t)n + (seq_t)pi;
}
