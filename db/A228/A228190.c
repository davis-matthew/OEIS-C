/*
 * A228190  a(n) = sum_{i=1..n} prime(i) + product_{i=1..n} prime(i).
 * https://oeis.org/A228190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A228190
    a(n) = sum_{i=1..n} prime(i) + product_{i=1..n} prime(i)
    Offset: 1  (a(1) = 2+2 = 4, a(2) = 2+3 + 2*3 = 11, etc.)
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state for incremental prime generation and caching */
static idx_t      cached_n    = 0;          /* number of primes stored so far   */
static seq_t      cached_sum  = 0;          /* sum of primes[0 .. cached_n-1]   */
static seq_t      cached_prod = 1;          /* product of primes[0 .. cached_n-1]*/
static uint64_t  *cached_primes = NULL;     /* array of primes                    */
static size_t     cached_cap  = 0;          /* allocated capacity for primes      */

/* ensure primes array can hold at least n entries */
static void ensure_capacity(idx_t n)
{
    if (cached_cap < (size_t)n) {
        uint64_t *tmp = (uint64_t *)realloc(cached_primes, (size_t)n * sizeof(uint64_t));
        if (!tmp) abort();            /* allocation failure */
        cached_primes = tmp;
        cached_cap    = (size_t)n;
    }
}

/* simple primality test using the primes already stored */
static bool is_prime_candidate(uint64_t cand)
{
    if (cand < 2) return false;
    for (size_t i = 0; i < cached_n; ++i) {
        uint64_t p = cached_primes[i];
        if (p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* main function required by the harness */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for 0, harmless */

    /* If we already have computed up to n, compute directly from cache */
    if (n <= cached_n) {
        seq_t sum  = 0;
        seq_t prod = 1;
        for (idx_t i = 0; i < n; ++i) {
            seq_t p = (seq_t)cached_primes[i];
            sum += p;
            prod *= p;
        }
        return sum + prod;
    }

    /* Ensure capacity and generate missing primes up to n */
    ensure_capacity(n);
    seq_t sum  = cached_sum;
    seq_t prod = cached_prod;
    uint64_t cand = (cached_n == 0) ? 2 : cached_primes[cached_n - 1] + 1;

    while (cached_n < n) {
        /* find next prime */
        while (!is_prime_candidate(cand)) ++cand;

        /* store prime */
        cached_primes[cached_n] = cand;

        /* update cumulative sum and product */
        seq_t p = (seq_t)cand;
        sum  += p;
        prod *= p;

        ++cached_n;
        ++cand;                         /* move to next candidate */
    }

    /* Cache updated values for future calls */
    cached_sum  = sum;
    cached_prod = prod;

    return sum + prod;
}
