/*
 * A044360  Numbers n such that string 2,8 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains28
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A044360 – Numbers n such that the decimal string "28" occurs in n but not in n‑1.
    The function returns the n‑th element of this sequence (n ≥ 1). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains28.h"

/* Ensure that *arr has space for at least 'needed' elements.
   (realloc is casted for C++ compatibility.) */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0 ? 64 : *cap << 1);
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

/* External linkage: sequence generator. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;        /* capacity of seq array           */
    static idx_t current_checked = 0; /* number up to which we have examined */
    static idx_t found_count = 0;      /* number of sequence values found */

    if (n == 0) return 0;            /* offset is 1 – undefined for 0 */

    /* Produce terms until we have at least n of them. */
    while (found_count < n) {
        ++current_checked;
        if (contains28(current_checked) && !contains28(current_checked - 1)) {
            ensure_capacity(&seq, &seq_cap, found_count + 1);
            seq[found_count] = (seq_t)current_checked; /* conversion ctor */
            ++found_count;
        }
    }
    return seq[n - 1];
}
