/*
 * A138009  a(n) = number of positive integers k, k <= n, where d(k) >= d(n);
 *          d(n) = number of positive divisors of n.
 * https://oeis.org/A138009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of k ≤ n with d(k) ≥ d(n) (d is divisor count)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint16_t *divcnt = NULL;     /* divisor counts, index 1..max */
static seq_t    *aseq   = NULL;     /* a(n) values, index 1..max */
static size_t   freq[2001] = {0};    /* frequency of each divisor count */
static idx_t    cache_max = 0;

/* Extend caches to hold values up to new_limit */
static void extend_cache(idx_t new_limit) {
    if (new_limit <= cache_max) return;

    /* Reallocate arrays to accommodate up to new_limit */
    divcnt = (uint16_t*)realloc(divcnt, (new_limit + 1) * sizeof(uint16_t));
    aseq   = (seq_t*)   realloc(aseq,   (new_limit + 1) * sizeof(seq_t));

    /* Compute divisor counts and a(n) for newly added indices */
    for (idx_t i = cache_max + 1; i <= new_limit; ++i) {
        uint16_t cnt = 1;
        idx_t n = i;
        for (idx_t p = 2; (idx_t)p * p <= n; ++p) {
            if (n % p == 0) {
                int exp = 0;
                while (n % p == 0) {
                    n /= p;
                    ++exp;
                }
                cnt *= (uint16_t)(exp + 1);
            }
        }
        if (n > 1) cnt *= 2;               /* remaining prime factor */
        divcnt[i] = cnt;

        /* Count how many numbers seen so far have fewer divisors than cnt */
        size_t sum_small = 0;
        for (uint16_t d = 1; d < cnt; ++d) {
            sum_small += freq[d];
        }
        aseq[i] = (seq_t)i - (seq_t)sum_small;
        freq[cnt] += 1;                    /* include this number for future queries */
    }
    cache_max = new_limit;
}

/* Public function */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for 0, but safe to return 0 */
    extend_cache(n);
    return aseq[n];
}
