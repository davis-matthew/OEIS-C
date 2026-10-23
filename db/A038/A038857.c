/*
 * A038857  Numbers ending with '2' that are the difference of two positive
 *          cubes.
 * https://oeis.org/A038857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038857: Numbers ending with '2' that are the difference of two positive cubes.
   Computed by generating all cube differences ending with 2, sorting, and deduplicating. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/cmp_seq_t.h"

/* Static storage for generated values */
static seq_t *data = NULL;          /* sorted unique cube differences ending with 2 */
static idx_t data_len = 0;          /* number of elements in data   */
static idx_t current_max_x = 2000;  /* current bound on x for generation   */
static bool built = false;          /* whether the sequence has been generated */

/* Helper function: build the sequence up to a given maximum x */
static void build_sequence_for_x(idx_t max_x) {
    /* Free any previously allocated memory */
    if (data != NULL) {
        free(data);
    }

    /* Estimate number of (x,y) pairs: max_x*(max_x-1)/2 */
    size_t approx_pairs = (size_t)max_x * (max_x - 1) / 2;
    size_t capacity = approx_pairs; /* at most one candidate per pair */
    data = (seq_t *)malloc(capacity * sizeof(seq_t));
    if (data == NULL) {
        /* Allocation failure: terminate. */
        abort();
    }
    size_t raw_len = 0;

    /* Generate all cube differences for 1 <= y < x <= max_x */
    for (idx_t x = 1; x <= max_x; ++x) {
        seq_t x_seq = (seq_t)x;
        seq_t x3 = x_seq * x_seq * x_seq;
        for (idx_t y = 1; y < x; ++y) {
            seq_t y_seq = (seq_t)y;
            seq_t y3 = y_seq * y_seq * y_seq;
            seq_t diff = x3 - y3;
            if ((diff % (seq_t)10) == (seq_t)2) {
                data[raw_len++] = diff;
            }
        }
    }

    /* Sort the raw list */
    qsort(data, raw_len, sizeof(seq_t), cmp_seq_t);

    /* Deduplicate in-place */
    size_t uniq_len = 0;
    for (size_t i = 0; i < raw_len; ++i) {
        if (uniq_len == 0 || cmp_seq_t(&data[i], &data[uniq_len - 1]) != 0) {
            data[uniq_len++] = data[i];
        }
    }

    data_len = uniq_len;
    current_max_x = max_x;
    built = true;
}

/* Main function requested by the build */
seq_t a(idx_t n) {
    /* Build the sequence lazily if not yet done */
    if (!built) {
        build_sequence_for_x(current_max_x);
    }
    /* Grow the generation if necessary */
    while (data_len <= n) {
        /* Double the bound on x to generate more candidates */
        current_max_x *= 2;
        build_sequence_for_x(current_max_x);
    }
    /* Return the indexed value */
    return data[n];
}
