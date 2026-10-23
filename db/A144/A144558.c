/*
 * A144558  Expansion of Product_{n >= 1}
 *          (1+q^(2*n-1))/((1-q^(4*n))*(1+q^(4*n-2))).
 * https://oeis.org/A144558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in * ∏_{i≥1} (1+q^{2i-1}) /
 *            ((1-q^{4i})(1+q^{4i-2})) * * The sequence is computed by
 *            expanding the product as a * generating function. Coefficients
 *            are stored in a static * cache and recomputed when a larger
 *            index is requested. * * This algorithm uses dynamic
 *            programming...
 * Range:     i8 n <= 23, i16 n <= 69, i32 n <= 215, i64 n <= 717,
 *            i128 n <= 1000, big any n
 * Helpers:   compute_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compute_seq.h"

/* ===== main sequence function ===== */
static seq_t *cache = NULL;        /* cached coefficients */
static idx_t cached_size = 0;      /* number of cached coefficients */

seq_t a(idx_t n)
{
    /* Precompute up to at least 10000 on first call */
    if (cache == NULL) {
        idx_t target = (n > 10000) ? n : 10000;
        cache = (seq_t*)malloc((size_t)(target + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* safety */
        compute_seq(cache, target);
        cached_size = target + 1;
    } else if (n >= cached_size) {
        /* Need more coefficients: recompute up to n */
        idx_t target = n;
        seq_t *new_cache = (seq_t*)malloc((size_t)(target + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* safety */
        compute_seq(new_cache, target);
        free(cache);
        cache = new_cache;
        cached_size = target + 1;
    }

    return cache[n];
}
