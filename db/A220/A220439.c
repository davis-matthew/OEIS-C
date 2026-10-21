/*
 * A220439  The Deléglise-Nicolas function h_2(n).
 * https://oeis.org/A220439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{p*q : p and q distinct primes, p+q <= n}
 * Range:     u8 n <= 35, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *g_primes = NULL;
static size_t   g_primes_cnt = 0;
static idx_t   g_max_n = 0;

seq_t a(idx_t n) {
    if (n < 5) return 0;           /* no valid pair for n < 5 */

    /* Recompute primes up to n if a larger n has been requested. */
    if ((idx_t)n > g_max_n) {
        size_t limit = (size_t)n;
        bool *sieve = (bool *)malloc(limit + 1);
        if (!sieve) return 0;                 /* out of memory */

        for (size_t i = 2; i <= limit; ++i) sieve[i] = true;
        for (size_t p = 2; p * p <= limit; ++p)
            if (sieve[p])
                for (size_t m = p * p; m <= limit; m += p)
                    sieve[m] = false;

        /* Count primes */
        size_t new_cnt = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (sieve[i]) ++new_cnt;

        uint64_t *new_primes = (uint64_t *)malloc(new_cnt * sizeof(uint64_t));
        if (!new_primes) {
            free(sieve);
            return 0;
        }
        size_t idx = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (sieve[i]) new_primes[idx++] = i;

        free(sieve);
        free(g_primes);
        g_primes = new_primes;
        g_primes_cnt = new_cnt;
        g_max_n = n;
    }

    /* Find the maximum product p*q with distinct primes p<q and p+q <= n */
    seq_t maxprod = 0;
    size_t i = 0;
    size_t j = g_primes_cnt - 1;
    while (i < j) {
        uint64_t pi = g_primes[i];
        uint64_t pj = g_primes[j];
        idx_t sum = (idx_t)pi + (idx_t)pj;
        if (sum <= n) {
            seq_t prod = (seq_t)pi * (seq_t)pj;
            if (prod > maxprod) maxprod = prod;
            ++i;
        } else {
            --j;
        }
    }
    return maxprod;
}
