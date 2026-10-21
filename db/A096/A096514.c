/*
 * A096514  Solutions to A096509[x]=3; number of prime-powers [including
 *          primes] in the neighborhood of x with Ceiling[Log[x]] radius
 *          equals 3.
 * https://oeis.org/A096514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th integer x such that the count of
   prime powers (including primes) in the interval
   [x – ceil(log(x)), x + ceil(log(x))] equals 3.
   The function builds a sieve of primes up to a fixed cutoff,
   marks all prime powers, builds a prefix sum array for quick
   interval counts, and then scans integers until the nth
   qualifying integer is reached. */

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static data structures, initialized once on first call */
    static int initialized = 0;
    static size_t MAX = 10000000; /* sufficiently large for n up to ~10^4 */
    static unsigned char *pp = NULL;      /* prime‑power flag array */
    static uint32_t *pref = NULL;        /* prefix sums of pp */

    if (!initialized) {
        /* Allocate arrays */
        pp = (unsigned char *)malloc(MAX + 1);
        if (!pp) return 0;                     /* out of memory */
        pref = (uint32_t *)malloc((MAX + 1) * sizeof(uint32_t));
        if (!pref) { free(pp); return 0; }

        /* Sieve of Eratosthenes for primes up to MAX */
        unsigned char *is_prime = (unsigned char *)malloc(MAX + 1);
        if (!is_prime) { free(pp); free(pref); return 0; }
        memset(is_prime, 1, MAX + 1);
        is_prime[0] = is_prime[1] = 0;
        for (size_t i = 2; i * i <= MAX; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= MAX; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Mark all prime powers (including primes themselves) */
        memset(pp, 0, MAX + 1);
        for (size_t i = 2; i <= MAX; ++i) {
            if (is_prime[i]) {
                pp[i] = 1;                       /* prime itself */
                if (i * i <= MAX) {
                    __uint128_t v = (__uint128_t)i * i;
                    while (v <= MAX) {
                        pp[(size_t)v] = 1;
                        if (v > ( (__uint128_t)MAX / i)) break;
                        v *= i;
                    }
                }
            }
        }
        free(is_prime);

        /* Build prefix sums for quick range queries */
        pref[0] = 0;
        for (size_t i = 1; i <= MAX; ++i)
            pref[i] = pref[i - 1] + pp[i];

        initialized = 1;
    }

    /* Scan integers until the nth qualifying integer is found */
    idx_t found = 0;
    for (idx_t x = 1; ; ++x) {
        /* radius = ceil(log_e(x)) */
        int radius = (int)ceil(log((double)x));
        idx_t L = (x > (idx_t)radius) ? x - (idx_t)radius : 1;
        idx_t R = x + (idx_t)radius;
        if (R > MAX) break;   /* safety: reserve enough bandwidth */
        uint32_t count = pref[R] - pref[L - 1];
        if (count == 3) {
            ++found;
            if (found == n)
                return (seq_t)x;
        }
    }

    return 0;   /* should never reach here for valid inputs */
}
