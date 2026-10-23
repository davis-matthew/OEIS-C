/*
 * A180241  Numbers whose square can be expressed as the sum of a square, a
 *          cube and a fourth power, all positive.
 * https://oeis.org/A180241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer a such that * a^2 = x^2 + y^3
 *            + z^4 for some positive integers x, y, z
 * Range:     u8 n <= 3, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   floor_sqrt, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t) */
#include "../utils/seq_realloc.h" /* seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* the next integer to test */
    static uint64_t next_val = 1;

    /* grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        seq_t val_sq = (seq_t)next_val * (seq_t)next_val;
        bool ok = false;

        /* loop over possible z */
        for (uint64_t z = 1; ; ++z) {
            seq_t z4 = (seq_t)z * (seq_t)z * (seq_t)z * (seq_t)z;
            if (z4 >= val_sq)
                break;

            seq_t r1 = val_sq - z4;

            /* loop over possible y */
            for (uint64_t y = 1; ; ++y) {
                seq_t y3 = (seq_t)y * (seq_t)y * (seq_t)y;
                if (y3 >= r1)
                    break;

                seq_t rem = r1 - y3;            /* x^2 */
                seq_t sqr = floor_sqrt(rem);
                if (sqr != 0 && sqr * sqr == rem) {
                    ok = true;
                    break;
                }
            }
            if (ok)
                break;
        }

        if (ok) {
            /* enlarge the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_val;
        }
        ++next_val;
    }

    return cache[n - 1];
}
