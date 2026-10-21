/*
 * A072360  One-sixth the area of the smallest primitive d-arithmetic
 *          triangle, where d=A072330(n).
 * https://oeis.org/A072360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = x'*y'/2 where (x',y') is the minimal primitive solution
 *            to * x^2 - 3*y^2 = d^2, with d = A072330(n). d is the * n-th
 *            integer all of whose prime factors are congruent to * 1 or 11
 *            (mod 12) and not divisible by 3. This function * generates terms
 *            on demand
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   d_is_allowed, floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"
#include "../utils/d_is_allowed.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_alloc = 0;
static idx_t next_d = 1;

static void grow_cache(size_t needed)
{
    if (cache_alloc < needed) {
        size_t new_cap = cache_alloc ? cache_alloc * 2 : 64;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_ptr;
        cache_alloc = new_cap;
    }
}

static seq_t compute_area_for_d(idx_t d)
{
    seq_t dval = d;
    for (idx_t y = 1; y <= d; ++y) {
        seq_t lhs = dval * dval + 3 * (seq_t)y * (seq_t)y;
        seq_t x = floor_sqrt(lhs);
        if (x * x == lhs && gcd(x, (seq_t)y) == 1) {
            return x * y / 2;
        }
    }
    /* Should never reach here for allowed d. */
    return 0;
}

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n terms. */
    while (cache_size < n) {
        /* Find next allowed d. */
        idx_t d;
        for (;;) {
            d = next_d++;
            if (!d_is_allowed(d)) continue;
            seq_t area = compute_area_for_d(d);
            /* area will be non-zero for allowed d. */
            if (area > 0) {
                grow_cache(cache_size + 1);
                cache[cache_size++] = area;
                break;
            }
        }
    }
    return cache[n - 1];
}
