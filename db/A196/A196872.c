/*
 * A196872  A007018(n) repeated A007018(n) times.
 * https://oeis.org/A196872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k_m where k_0 = 1, k_m = 1 + Σ_{i=0}^{m-1} k_i² , and the
 *            sequence a is defined by repeating each k_m exactly k_m times.
 *            For example: k: 1, 2, 6, 42, 1806, ... a: 1, 2, 2, 6, 6, 6, 6,
 *            6, 6, 42, 42, … (offset 0)
 * Range:     u8 n <= 50, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* --- internal state ---------------------------------------------------- */
static seq_t *k_vals = NULL;          /* values k_m                     */
static idx_t *cum_len = NULL;        /* cumulative lengths Σk_i up to i*/
static size_t k_count = 0;           /* number of k values stored     */
static size_t k_cap   = 0;           /* allocated capacity for k_vals  */
static seq_t sum_squares = 0;        /* Σ k_i²                         */

/* Grow the k_vals array to at least new_cap elements */
static void grow_k_vals(size_t new_cap)
{
    if (new_cap <= k_cap)
        return;
    seq_t *new_arr = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < k_count; ++i)
        new_arr[i] = k_vals[i];
    if (k_vals)
        seq_free_array(k_vals);
    k_vals = new_arr;
    k_cap   = new_cap;
}

/* Ensure that the internal tables contain enough terms so that
   cum_len[k_count-1] > n. */
static void ensure_up_to(idx_t n)
{
    /* first initialise if empty */
    if (k_count == 0) {
        k_cap = 8;                          /* start with a small capacity */
        k_vals = seq_alloc_new_array(k_cap);
        cum_len = (idx_t *)malloc(k_cap * sizeof(idx_t));

        k_vals[0] = 1;                      /* k_0 = 1 */
        cum_len[0] = 1;                     /* appears once */
        sum_squares = (seq_t)1 * (seq_t)1;  /* Σk_i² = 1² */
        k_count = 1;
    }

    /* grow until the cumulative length exceeds the requested index */
    while (cum_len[k_count - 1] <= n) {
        /* grow array capacities if needed */
        if (k_count == k_cap)
            grow_k_vals(k_cap * 2);

        /* compute next k value */
        seq_t next_k = (seq_t)1 + sum_squares;

        /* update cumulative sum of squares */
        sum_squares += next_k * next_k;

        /* store the value */
        k_vals[k_count] = next_k;

        /* compute new cumulative length */
        idx_t prev_cum = cum_len[k_count - 1];
        idx_t new_cum  = prev_cum + (idx_t)next_k;
        cum_len[k_count] = new_cum;

        ++k_count;
    }
}

/* Binary search for the first index i with cum_len[i] > n */
static size_t find_k_idx(idx_t n)
{
    size_t lo = 0, hi = k_count - 1;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (cum_len[mid] <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    size_t idx = find_k_idx(n);
    return k_vals[idx];
}
