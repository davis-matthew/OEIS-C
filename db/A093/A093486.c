/*
 * A093486  Least k >= 0 such that k! + n is a square, or -1 if no such number
 *          exists.
 * https://oeis.org/A093486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  less, sign
 * Formula:   a(n) = least k ≥ 0 such that k! + n – 1 is a perfect square, or
 *            -1 if no such k exists. The sequence has offset 1: a(1) is the
 *            first term
 * Range:     i8 n <= 4, i16 n <= 4, i32 n <= 4, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   floor_sqrt, max_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/max_k.h"

/* Cache of factorials up to max_k.  The cache is created lazily. */
static seq_t *fact_cache = NULL;
static size_t fact_cache_size = 0;

static void init_fact_cache(void)
{
    if (fact_cache) return;

    fact_cache_size = max_k();
    fact_cache = (seq_t*)malloc((fact_cache_size + 1) * sizeof(seq_t));
    if (!fact_cache) return;            /* allocation failed – abort silently */

    fact_cache[0] = (seq_t)1;
    for (size_t k = 1; k <= fact_cache_size; ++k) {
        fact_cache[k] = fact_cache[k - 1] * (seq_t)k;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)-1;   /* out of range – not used in tests */

    init_fact_cache();
    if (!fact_cache) return (seq_t)-1;  /* safety */

    seq_t offset = (seq_t)(n - 1);
    for (size_t k = 0; k <= fact_cache_size; ++k) {
        seq_t target = fact_cache[k] + offset;
        seq_t m = floor_sqrt(target);
        if (m * m == target) {
            return (seq_t)k;
        }
    }
    return (seq_t)-1;
}
