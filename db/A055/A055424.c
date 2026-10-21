/*
 * A055424  Number of points in N^n of norm <= 9.
 * https://oeis.org/A055424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of vectors in N^n whose Euclidean norm <= 9. It
 *            counts the non‑negative integer tuples (x1,…,xn) with Σ xi² <=
 *            81
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 33, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;
static seq_t *a_vals  = NULL;
static size_t a_len   = 0;              /* allocated length of a_vals   */
static idx_t  max_n   = 0;              /* largest n already computed  */

static const int squares[10] = {0,1,4,9,16,25,36,49,64,81};

/* Ensure that a_vals contains values up to a(n) and that the DP arrays
   are ready for the current number of dimensions.  */
static void ensure_computed(idx_t n) {
    if (dp_cur == NULL) {
        /* Initial allocation for DP arrays of size 82 (0…81). */
        dp_cur  = (seq_t*)calloc(82, sizeof(seq_t));
        dp_next = (seq_t*)calloc(82, sizeof(seq_t));
        if (!dp_cur || !dp_next) return;        /* allocation failed */

        /* Initial value for dimension 0. */
        dp_cur[0] = 1;                           /* only the zero vector */
        a_vals = (seq_t*)malloc(sizeof(seq_t));
        if (!a_vals) return;

        a_vals[0] = 1;                           /* a(0)=1 */
        a_len   = 1;
        max_n   = 0;
        return;
    }

    if (n <= max_n) return;                     /* already computed */

    for (idx_t dim = max_n + 1; dim <= n; ++dim) {
        /* Zero dp_next for this dimension. */
        for (int s = 0; s <= 81; ++s) dp_next[s] = 0;

        /* Transition: add one more coordinate (0…9). */
        for (int x = 0; x <= 9; ++x) {
            int sq = squares[x];
            for (int s = sq; s <= 81; ++s) {
                dp_next[s] += dp_cur[s - sq];
            }
        }

        /* Sum over all squared norms to obtain a(dim). */
        seq_t val = 0;
        for (int s = 0; s <= 81; ++s) val += dp_next[s];

        /* Grow a_vals array if necessary. */
        if (a_len <= dim) {
            size_t new_len = a_len * 2;
            if (new_len <= dim) new_len = dim + 1;
            seq_t *tmp = (seq_t*)malloc(new_len * sizeof(seq_t));
            if (!tmp) return;                    /* allocation failure */
            for (size_t i = 0; i < a_len; ++i) tmp[i] = a_vals[i];
            free(a_vals);
            a_vals = tmp;
            a_len  = new_len;
        }

        a_vals[dim] = val;
        max_n = dim;

        /* Prepare for the next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }
}

seq_t a(idx_t n) {
    ensure_computed(n);
    return a_vals[n];
}
