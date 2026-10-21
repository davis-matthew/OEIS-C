/*
 * A079350  "Look and Say" (or LS) transform of A079342(n). These terms are
 *          such that A079342(n) divides a(n).
 * https://oeis.org/A079350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 28, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   compute_ls_and_check
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) = LS(A079342(n))
   Computed by scanning integers in increasing order, selecting those
   n that divide LS(n).  For each such n the value LS(n) is stored
   as a(n).  Resulting array of a(n) values is memoised for fast
   repeated calls. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/compute_ls_and_check.h"

/* memoised storage for computed a(n) values */
static seq_t *a_vals = NULL;
static size_t a_cap = 0;
static size_t a_len = 0;
static idx_t last_candidate = 0;

/* ensure capacity for a_vals at least need */
static void ensure_capacity(size_t need)
{
    if (need <= a_cap)
        return;
    size_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) {
        /* unable to allocate, terminate */
        exit(1);
    }
    if (a_vals) {
        memcpy(new_arr, a_vals, a_len * sizeof(seq_t));
        free(a_vals);
    }
    a_vals = new_arr;
    a_cap = new_cap;
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    while (a_len < n) {
        ++last_candidate;
        seq_t ls_val;
        if (compute_ls_and_check(last_candidate, &ls_val)) {
            ensure_capacity(a_len + 1);
            a_vals[a_len++] = ls_val;
        }
    }
    return a_vals[n - 1];
}
