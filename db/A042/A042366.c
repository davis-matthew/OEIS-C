/*
 * A042366  Numerators of continued fraction convergents to sqrt(710).
 * https://oeis.org/A042366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2558*a(n-8) - a(n-16), for n ≥ 16
 * Initial values (from continued‑fraction convergents of √710):
 *   a(0)=26, a(1)=27, a(2)=53,  a(3)=80,  a(4)=373,  a(5)=453,
 *   a(6)=826, a(7)=1279, a(8)=67334, a(9)=68613, a(10)=135947, a(11)=204560,
 *   a(12)=954187, a(13)=1158747, a(14)=2112934, a(15)=3271681
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size */
    static idx_t cache_len = 0;   /* number of values initialized */

    /* If cache not initialized, compute the first 16 terms via the
       continued‑fraction algorithm for √710. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* Out of memory; return 0 as a safe default. */

        /* Compute the first 16 convergent numerators for √710. */
        const idx_t D = 710;          /* the quadratic radical */
        const idx_t a0 = 26;          /* floor(√710) */

        /* Convergent recurrence: p[-1] = 1, p[0] = a0 */
        cache[0] = (seq_t)a0;
        seq_t p_prev = 1;          /* p[-1] */
        seq_t p_curr = a0;         /* p[0] */

        /* Continued‑fraction variables */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;

        for (idx_t i = 1; i < 16; ++i) {
            /* Compute next CF term a_i */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;

            /* Update convergent numerator */
            seq_t p_next = (seq_t)a * p_curr + p_prev;
            cache[i] = p_next;

            /* Shift for next iteration */
            p_prev = p_curr;
            p_curr = p_next;
        }

        cache_len = 16;  /* we have terms 0..15 stored */
    }

    /* Grow cache if the requested index exceeds current capacity. */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* Out of memory; safe default. */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n if necessary. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 16) {
            /* All bases already computed */
        } else {
            cache[i] = (seq_t)2558 * cache[i - 8] - cache[i - 16];
        }
    }
    cache_len = n + 1;

    return cache[n];
}
