/*
 * A031406  Numbers k such that the continued fraction for sqrt(k) has even
 *          period 2*m and the m-th term of the periodic part is 3.
 * https://oeis.org/A031406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build the OEIS sequence A031406:
   Numbers k such that the continued‑fraction for √k has even period 2*m
   and the m‑th term of the periodic part is 3.
   The function does a brute‑force search, memoises results in a static
   cache, and works for any build where seq_t is an integer type. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* floor integer square root for 64‑bit values (used only to skip squares) */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;        /* next integer to test */

    if (n > cache_len) {
        /* ensure cache can hold at least n terms */
        if (cache_cap == 0) {
            cache_cap = 32;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }
        while (cache_len < n) {
            /* grow cache for new entries */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }

            idx_t k = next_k++;
            if (k == 0) continue;   /* overflow protection */

            /* skip perfect squares */
            idx_t sq = int_sqrt(k);
            if (sq * sq == k) continue;

            idx_t a0 = sq;
            idx_t m = 0, d = 1, a_term = a0;

            /* period storage */
            size_t period_cap = 4;
            size_t period_len = 0;
            idx_t *period = (idx_t *)malloc(period_cap * sizeof(idx_t));
            if (!period) { /* allocation failure – stop search */
                return 0;
            }

            while (true) {
                /* use 128‑bit intermediates to avoid overflow */
                __uint128_t dm = (__uint128_t)d * a_term;
                m = (idx_t)(dm - m);
                __uint128_t diff = (__uint128_t)k - (__uint128_t)m * m;
                d = (idx_t)(diff / d);
                a_term = (idx_t)((a0 + m) / d);

                if (period_len == period_cap) {
                    period_cap *= 2;
                    idx_t *tmp = (idx_t *)realloc(period, period_cap * sizeof(idx_t));
                    if (!tmp) { free(period); return 0; }
                    period = tmp;
                }
                period[period_len++] = a_term;

                if (a_term == a0 + a0)  /* period terminator */
                    break;
            }

            bool ok = false;
            if ((period_len & 1) == 0) {              /* even period */
                idx_t mid_index = period_len / 2 - 1; /* 0‑based */
                if (period[mid_index] == 3) ok = true;
            }

            free(period);

            if (ok) {
                cache[cache_len++] = (seq_t)k;
            }
        }
    }

    return cache[n - 1];
}
