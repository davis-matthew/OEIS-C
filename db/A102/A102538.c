/*
 * A102538  Numbers m not of the form k*(k+2) that have a single '1' in the
 *          periodic part of the continued fraction of sqrt(n).
 * https://oeis.org/A102538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   isqrt, period_one_count, resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102538: numbers m not of the form k(k+2) that contain
 * exactly one '1' in the periodic part of the continued fraction
 * representation of sqrt(m).  Offset = 1. */
#include <stdbool.h>
#include "../utils/isqrt.h"
#include "../utils/resize_array.h"
#include "../utils/period_one_count.h"

/* Public sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1 */

    /* static cache of computed terms */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t cache_cap   = 0;
    static seq_t last_m      = 0;    /* last integer examined */

    while (cache_size < n) {
        seq_t m = last_m + 1UL;               /* next integer */

        /* skip perfect squares */
        seq_t sqrt_m = isqrt(m);
        if (sqrt_m * sqrt_m == m) {
            last_m = m;
            continue;
        }

        /* skip numbers of the form t^2 - 1 (i.e. m+1 is a square) */
        seq_t sqrt_m1 = isqrt(m + 1);
        if (sqrt_m1 * sqrt_m1 == m + 1) {
            last_m = m;
            continue;
        }

        /* count 1's in the periodic part of sqrt(m) */
        if (period_one_count(m, sqrt_m) == 1) {
            /* enlarge cache if necessary */
            if (cache_cap <= cache_size) {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = resize_array(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_size++] = m;
            if (cache_size == n) {
                last_m = m;
                return m;
            }
        }
        last_m = m;
    }

    /* should never reach here */
    return cache[n - 1];
}
