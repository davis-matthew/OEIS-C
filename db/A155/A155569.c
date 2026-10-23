/*
 * A155569  Intersection of A002479 inter A002481: N = a^2 + 2b^2 = c^2 + 6d^2
 *          for some integers a,b,c,d.
 * https://oeis.org/A155569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155569
 * a(n) is the n-th smallest integer that can be represented
 * both as a^2 + 2b^2 and as c^2 + 6d^2.
 * This implementation uses a brute‑force sieve that grows until
 * the requested term is found.                                     */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Forward declaration of the build routine */
static void build_intersection(idx_t limit);

/* Static state for memoization */
static bool *rep_a = NULL;
static bool *rep_b = NULL;
static seq_t *intersection = NULL;
static size_t inter_size = 0;
static size_t inter_capacity = 0;
static idx_t current_limit = 0;

/* Ensure we have at least n terms precomputed.
   If not, double the limit and rebuild until enough.            */
static void ensure_n(idx_t n)
{
    if (inter_size >= (size_t)n)
        return;

    idx_t new_limit = current_limit ? current_limit * 2 : 500000;

    /* Build until we have at least n terms. */
    while (inter_size < (size_t)n) {
        build_intersection(new_limit);
        if (inter_size >= (size_t)n)
            break;
        new_limit *= 2;
    }
}

/* Build the intersection up to the given limit.
   All arrays are rebuilt from scratch.                          */
static void build_intersection(idx_t limit)
{
    /* Allocate marker arrays */
    size_t sz = (size_t)limit + 1;
    rep_a = (bool *)calloc(sz, sizeof(bool));
    rep_b = (bool *)calloc(sz, sizeof(bool));
    if (!rep_a || !rep_b)
        exit(EXIT_FAILURE);

    /* Representation a^2 + 2b^2 */
    idx_t a_max = (idx_t)floor_sqrt((seq_t)limit);
    for (idx_t a = 0; a <= a_max; ++a) {
        idx_t a2 = a * a;
        idx_t rem = limit - a2;
        idx_t b_max = (idx_t)floor_sqrt((seq_t)(rem / 2));
        for (idx_t b = 0; b <= b_max; ++b) {
            idx_t v = a2 + 2 * b * b;
            rep_a[v] = true;
        }
    }

    /* Representation c^2 + 6d^2 */
    idx_t c_max = (idx_t)floor_sqrt((seq_t)limit);
    for (idx_t c = 0; c <= c_max; ++c) {
        idx_t c2 = c * c;
        idx_t rem = limit - c2;
        idx_t d_max = (idx_t)floor_sqrt((seq_t)(rem / 6));
        for (idx_t d = 0; d <= d_max; ++d) {
            idx_t v = c2 + 6 * d * d;
            rep_b[v] = true;
        }
    }

    /* Gather the intersection into a sorted array */
    free(intersection);
    intersection = NULL;
    inter_size = 0;
    inter_capacity = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (rep_a[i] && rep_b[i]) {
            if (inter_size == inter_capacity) {
                inter_capacity = inter_capacity ? inter_capacity * 2 : 1024;
                intersection = (seq_t *)realloc(intersection,
                                                inter_capacity * sizeof(seq_t));
                if (!intersection)
                    exit(EXIT_FAILURE);
            }
            intersection[inter_size++] = (seq_t)i;
        }
    }

    /* Clean up marker arrays */
    free(rep_a);
    free(rep_b);
    rep_a = rep_b = NULL;

    current_limit = limit;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) is undefined; return 0 as safety */
        return (seq_t)0;
    ensure_n(n);
    return intersection[n - 1];
}
