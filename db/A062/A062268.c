/*
 * A062268  Numbers k such that floor(k*e) is a square.
 * https://oeis.org/A062268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062268: Numbers k such that floor(k·e) is a square.
   The terms are generated on demand and cached.
   The algorithm uses rational bounds:
        D     = 10^10
        N_LOW = floor(e·D)     = 27182818284
        N_HIGH= N_LOW + 1      = 27182818285
   For each m (square root) we seek k satisfying
        m² ≤ k·e < m²+1
   using only integer arithmetic on the bounds.  Any integer k that
   satisfies both inequalities is a term of the sequence. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    const seq_t D      = 10000000000ULL;      /* 10^10 */
    const seq_t N_LOW  = 27182818284ULL;
    const seq_t N_HIGH = N_LOW + 1;

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  processed_m = 0;   /* next square root to examine */

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    while (cache_len <= (size_t)n) {
        seq_t m  = (seq_t)processed_m;
        seq_t m2 = m * m;
        seq_t num = m2 * D;

        /* bounds for k */
        seq_t lower = (num + N_HIGH - 1) / N_HIGH;   /* ceil(num / N_HIGH) */
        seq_t upper = (num + N_LOW  - 1) / N_LOW;    /* ceil(num / N_LOW) */

        if (lower <= upper) {
            for (seq_t k = lower; k <= upper; ++k) {
                /* inequality 1: k·N_LOW ≥ num */
                if (k * N_LOW >= num) {
                    seq_t upper_num = (m2 + 1) * D;
                    /* inequality 2: k·N_HIGH < (m²+1)·D */
                    if (k * N_HIGH < upper_num) {
                        if (cache_len >= cache_cap) {
                            cache_cap <<= 1;
                            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                            if (!tmp) abort();
                            cache = tmp;
                        }
                        cache[cache_len++] = k;
                        break;
                    }
                }
            }
        }
        /* Increment m regardless of whether a term was found. */
        ++processed_m;
    }

    return cache[n];
}
