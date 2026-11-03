/*
 * A190097  Number of rhombuses on a (n+1)X9 grid.
 * https://oeis.org/A190097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of rhombuses on a (n+1)×9 grid. For n ≤ 51 the
 *            value is obtained by a direct enumeration of all rhombuses. For
 *            n > 51 the known linear recurrence a(n) = 2*a(n-1) - 2*a(n-3) +
 *            a(n-4) is used. In both cases the sequence is stored in a static
 *            cache so that calls in any order are efficient
 * Range:     u8 n <= 2, u16 n <= 53, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: standard, pass, 58 terms through n = 199 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* count rhombuses on an (n+1)×9 grid, n ≤ 51 */
static seq_t _count_rhombuses(idx_t n)
{
    /* grid indices: x in [0, n], y in [0, 8] */
    const int max_y = 8;  /* height-1 */

    /* vectors: all (dx,dy) with dx∈[-n,n], dy∈[-8,8] except (0,0) */
    static int dx[2000];
    static int dy[2000];
    static seq_t len[2000];
    int vec_cnt = 0;
    for (int dxv = -(int)n; dxv <= (int)n; ++dxv) {
        for (int dyv = -8; dyv <= 8; ++dyv) {
            if (dxv == 0 && dyv == 0) continue;
            dx[vec_cnt]   = dxv;
            dy[vec_cnt]   = dyv;
            len[vec_cnt]  = (seq_t)dxv * dxv + (seq_t)dyv * dyv;
            ++vec_cnt;
        }
    }

    seq_t total = 0;

    /* Enumerate unordered pairs of distinct vectors with equal length
       and non‑collinear.  For each pair we compute the number of valid
       base points p = (x,y) such that p, p+v, p+w, p+v+w lie inside
       the grid.  Each rhombus is counted once per vertex, so it
       appears 4 times in the sum.  Finally we divide by 4. */
    for (int i = 0; i < vec_cnt; ++i) {
        int dx1 = dx[i];
        int dy1 = dy[i];
        seq_t l1 = len[i];
        for (int j = i + 1; j < vec_cnt; ++j) {
            if (len[j] != l1) continue;               /* different lengths */
            int dx2 = dx[j];
            int dy2 = dy[j];
            if (dx1 * dy2 - dy1 * dx2 == 0) continue;  /* collinear */

            /* bounding box for base coordinates */
            int min_dx = 0, max_dx = 0;
            int sum_dx = dx1 + dx2;
            if (dx1 < min_dx)        min_dx = dx1;
            if (dx2 < min_dx)        min_dx = dx2;
            if (sum_dx < min_dx)     min_dx = sum_dx;

            if (dx1 > max_dx)        max_dx = dx1;
            if (dx2 > max_dx)        max_dx = dx2;
            if (sum_dx > max_dx)     max_dx = sum_dx;

            int min_dy = 0, max_dy = 0;
            int sum_dy = dy1 + dy2;
            if (dy1 < min_dy)        min_dy = dy1;
            if (dy2 < min_dy)        min_dy = dy2;
            if (sum_dy < min_dy)     min_dy = sum_dy;

            if (dy1 > max_dy)        max_dy = dy1;
            if (dy2 > max_dy)        max_dy = dy2;
            if (sum_dy > max_dy)     max_dy = sum_dy;

            int lower_x = (min_dx < 0) ? -min_dx : 0;
            int upper_x = (int)n - max_dx;           /* 0 ≤ x ≤ n - max_dx */

            int lower_y = (min_dy < 0) ? -min_dy : 0;
            int upper_y = max_y - max_dy;            /* 0 ≤ y ≤ 8 - max_dy */

            if (upper_x < lower_x || upper_y < lower_y) continue;

            int width_x = upper_x - lower_x + 1;
            int width_y = upper_y - lower_y + 1;

            total += (seq_t)width_x * (seq_t)width_y;
        }
    }

    /* Each rhombus counted 4 times (once per vertex). */
    return total / (seq_t)4;
}

/* Main routine: returns a(n). */
seq_t a(idx_t n)
{
    /* dynamically grow cache up to maximum n requested */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t highest_computed = 0;

    if (n + 1 > cache_size) {
        size_t new_size = n + 1;
        seq_t *new_cache = mem_new(seq_t, new_size);
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            mem_free(cache);
        }
        cache      = new_cache;
        cache_size = new_size;
    }

    for (idx_t i = highest_computed + 1; i <= n; ++i) {
        if (i <= 51) {
            cache[i] = _count_rhombuses(i);
        } else {
            cache[i] = 2 * cache[i - 1]
                     - 2 * cache[i - 3]
                     + cache[i - 4];
        }
    }
    if (n > highest_computed)
        highest_computed = n;

    return cache[n];
}
