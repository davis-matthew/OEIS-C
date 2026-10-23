/*
 * A031518  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 20.
 * https://oeis.org/A031518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: numbers k such that the continued fraction for √k
   has even period and, after deleting the last term of the period,
   the central term of the remaining sequence equals 20. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Integer square root for idx_t (assumed unsigned 64‑bit). */
static inline idx_t int_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = (n >> 1) + 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        else if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;   /* floor(sqrt(n)) */
}

/* Maximum possible period length for the range of n we consider. */
#define MAX_PERIOD 4096

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache = NULL;      /* dynamically grown array of found terms   */
    static size_t cache_len = 0;     /* number of terms currently cached        */
    static size_t cache_cap = 0;     /* capacity of the cache array              */
    static idx_t next_k = 2;         /* next integer to test                     */

    /* Grow the cache when needed */
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n terms cached */
    while (cache_len < n) {
        /* Skip perfect squares */
        idx_t a0 = int_sqrt_u64(next_k);
        if (a0 * a0 == next_k) {
            ++next_k;
            continue;
        }

        /* Continued fraction expansion of √next_k */
        idx_t m = 0, d = 1, a_cur = a0;
        int terms[MAX_PERIOD];
        int len = 0;

        while (true) {
            m = d * a_cur - m;
            d = (next_k - m * m) / d;
            a_cur = (a0 + m) / d;
            if (len >= MAX_PERIOD) break;  /* safety guard */
            terms[len++] = (int)a_cur;
            if (a_cur == 2 * a0) break;     /* end of period */
        }

        /* Check the property: even period, central term 20 */
        if ((len % 2) == 0) {
            int mid_index = len / 2 - 1;   /* zero‑based */
            if (terms[mid_index] == 20) {
                cache[cache_len++] = (seq_t)next_k;
            }
        }

        ++next_k;
    }

    return cache[n - 1];
}
