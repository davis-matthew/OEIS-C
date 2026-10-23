/*
 * A020673  Numbers of form x^2 + 10 y^2.
 * https://oeis.org/A020673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   _floor_isqrt_u64, cmp_seq_t, free_seq, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers of form x^2 + 10 y^2 (OEIS A020673).
   This routine generates all values x^2+10y^2 up to an adaptive
   bound big enough to contain the n‑th term, sorts them, removes
   duplicates and stores the sorted list in a static cache.  Future
   calls reuse the cache and expand it only if a larger n is requested.
   The bound is increased exponentially until the desired length is
   achieved. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/free_seq.h"
#include "../utils/cmp_seq_t.h"
#include "../utils/_floor_isqrt_u64.h"

static seq_t *cache = NULL;
static idx_t cached_len = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache holds at least n terms */
    if (n > cached_len) {
        seq_t B = (seq_t)n * 10 + 100;          /* initial bound */
        while (1) {
            /* Limits for x and y so that x^2+10y^2 ≤ B */
            idx_t x_max = (idx_t)_floor_isqrt_u64((uint64_t)B);
            idx_t y_max = (idx_t)_floor_isqrt_u64((uint64_t)(B / 10));
            size_t cap = (size_t)(x_max + 1) * (size_t)(y_max + 1);

            /* Temporary array for raw values */
            seq_t *tmp = NULL;
            size_t tmp_cap = 0;
            grow_seq(&tmp, &tmp_cap, cap);

            size_t cnt = 0;
            for (idx_t y = 0; y <= y_max; ++y) {
                seq_t y2 = (seq_t)y * (seq_t)y * 10;
                for (idx_t x = 0; x <= x_max; ++x) {
                    seq_t val = (seq_t)x * (seq_t)x + y2;
                    if (val > B) break;          /* further x will only grow */
                    tmp[cnt++] = val;
                }
            }

            /* Sort and remove duplicates */
            qsort(tmp, cnt, sizeof(seq_t),
                  (int (*)(const void*,const void*))cmp_seq_t);
            size_t uniq = 0;
            for (size_t i = 0; i < cnt; ++i) {
                if (i == 0 || tmp[i] != tmp[i-1]) {
                    tmp[uniq++] = tmp[i];
                }
            }

            if (uniq >= n) {
                /* Replace the old cache */
                if (cache) free_seq(cache);
                cache = tmp;
                cached_len = uniq;
                break;
            } else {
                /* Not enough terms – discard and enlarge B */
                free_seq(tmp);
                B *= 2;
            }
        }
    }
    return cache[n - 1];
}
