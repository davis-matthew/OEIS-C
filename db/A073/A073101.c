/*
 * A073101  Number of integer solutions (x,y,z) to 4/n = 1/x + 1/y + 1/z
 *          satisfying 0 < x < y < z.
 * https://oeis.org/A073101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of integer triples (x,y,z) with 0 < x < y < z
 * satisfying 4/n = 1/x + 1/y + 1/z.
 * The implementation enumerates all admissible triples by iterating over
 * x and y and deriving z from the equation.
 * Complexity ≈ O(n log n) for n up to 10000, well below the limits.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache for already computed a(n) */
static idx_t cache_size = 0;         /* current free size (max index + 1) */

/* compute a(k) for all k in [old_size, upto] */
static void compute_upto(idx_t upto) {
    /* extend cache */
    seq_t *new_cache = (seq_t *)realloc(cache, (upto + 1) * sizeof(seq_t));
    if (!new_cache) return; /* allocation failure, but problem spec assumes success */
    cache = new_cache;
    /* compute new values */
    for (idx_t k = cache_size; k <= upto; ++k) {
        unsigned long long count = 0;
        /* x ranges: ceil(k/4)+1 <= x <= floor(3k/4) */
        idx_t x_start = k / 4 + 1;
        idx_t x_end   = (k * 3) / 4;
        if (x_end < x_start) {
            cache[k] = 0;
            continue;
        }
        for (idx_t x = x_start; x <= x_end; ++x) {
            long long denom_coef = 4LL * (long long)x - (long long)k; /* 4x - k > 0 */
            if (denom_coef <= 0) continue;
            long long nx = (long long)k * (long long)x;
            /* minimal y such that denominator > 0 */
            long long y_min = (nx + denom_coef - 1) / denom_coef;          /* ceil(n*x/(4x - n)) */
            if (y_min < (long long)(x + 1)) y_min = x + 1;
            /* maximal y such that z > y holds */
            long long ymax = (2LL * (long long)k * (long long)x) / (denom_coef);
            if (y_min > ymax) continue; /* no suitable y */
            for (idx_t y = (idx_t)y_min; y <= (idx_t)ymax; ++y) {
                unsigned long long num = (unsigned long long)k * (unsigned long long)x * (unsigned long long)y;
                long long den = 4LL * (long long)x * (long long)y
                                - (long long)k * ((long long)y + (long long)x);
                if (den <= 0) continue; /* safety, should not happen after y_min */
                if (num % den != 0) continue;
                unsigned long long z = num / den;
                if (z <= (unsigned long long)y) continue; /* must be strictly larger */
                /* x < y < z automatically satisfied by construction */
                ++count;
            }
        }
        cache[k] = (seq_t)count;
    }
    cache_size = upto + 1;
}

seq_t a(idx_t n) {
    if (n >= cache_size) {
        compute_upto(n);
    }
    return cache[n];
}
