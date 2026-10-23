/*
 * A156380  Number of 3X3 arrays of squares of integers, symmetric about main
 *          diagonal, with all rows summing to n.
 * https://oeis.org/A156380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3×3 arrays of integer squares, symmetric about
 *            the main diagonal, with all row sums equal to n
 * Range:     u8 n <= 63, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t countWays(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t count = 0;

    for (idx_t d = 0; ((seq_t)d * (seq_t)d) <= n_t; d++) {
        seq_t dd = (seq_t)d * (seq_t)d;
        for (idx_t e = 0; ((seq_t)e * (seq_t)e) <= (n_t - dd); e++) {
            seq_t ee = (seq_t)e * (seq_t)e;
            /* dd + ee <= n_t by loop condition */
            for (idx_t f = 0; ((seq_t)f * (seq_t)f) <= n_t; f++) {
                seq_t ff = (seq_t)f * (seq_t)f;
                if (dd + ff > n_t || ee + ff > n_t) break;    /* avoid further f */

                seq_t t1 = n_t - dd - ee;
                seq_t r1 = floor_sqrt(t1);
                if (r1 * r1 != t1) continue;

                seq_t t2 = n_t - dd - ff;
                seq_t r2 = floor_sqrt(t2);
                if (r2 * r2 != t2) continue;

                seq_t t3 = n_t - ee - ff;
                seq_t r3 = floor_sqrt(t3);
                if (r3 * r3 != t3) continue;

                count++;
            }
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = countWays(0);
    }

    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (size_t i = cache_size; i <= n; ++i) {
            cache[i] = countWays((idx_t)i);
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
