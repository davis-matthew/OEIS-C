/*
 * A194626  Numbers n such that next term of Hofstadter's Q-sequence differs
 *          by 1.
 * https://oeis.org/A194626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 142, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A194626: a(n) = the n such that Hofstadter Q(n+1)-Q(n)=1.
   Computed by iteratively evaluating the Q‑sequence and recording the
   indices where the increment is exactly 1. */
#include <stdlib.h>

/* Ensure that the dynamic array *arr has space for at least needed elements.
   Returns 1 on success, 0 on allocation failure. */
static int grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return 1;
    size_t new_cap = *cap ? *cap * 2 : 128;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) return 0;
    *arr = tmp;
    *cap = new_cap;
    return 1;
}

seq_t a(idx_t n)
{
    /* ---------- static caches ---------- */
    static seq_t *q_vals   = NULL; /* Q(1) stored at q_vals[0] */
    static size_t q_cap   = 0;
    static size_t q_len   = 0;

    static seq_t *inc_vals = NULL; /* inc_vals[0] == first n with Q(n+1)=Q(n)+1 */
    static size_t inc_cap  = 0;
    static size_t inc_len  = 0;

    /* Keep computing Q and recording indices until we have at least n entries. */
    while (inc_len < (size_t)n) {

        /* Grow arrays as needed before the next computation. */
        if (!grow_seq(&q_vals, &q_cap, q_len + 1)) return 0;
        if (!grow_seq(&inc_vals, &inc_cap, inc_len + 1)) return 0;

        /* Initialise the first two Q values on first invocation. */
        if (q_len == 0) q_vals[q_len++] = (seq_t)1; /* Q(1) */
        if (q_len == 1) q_vals[q_len++] = (seq_t)1; /* Q(2) */

        /* Next Q index (1‑based). */
        idx_t k = (idx_t)q_len + 1;

        seq_t next_q;
        if (k <= 2) {
            next_q = (seq_t)1;
        } else {
            /* Q(k) = Q(k - Q(k-1)) + Q(k - Q(k-2)) */
            idx_t idx1 = k - (idx_t)q_vals[k - 2]; /* Q(k-1) */
            idx_t idx2 = k - (idx_t)q_vals[k - 3]; /* Q(k-2) */
            next_q = q_vals[idx1 - 1] + q_vals[idx2 - 1];
        }

        /* Store the new Q value. */
        q_vals[q_len++] = next_q;

        /* Record the index n where the increment is exactly 1. */
        if (q_len >= 2) {
            seq_t diff = q_vals[q_len - 1] - q_vals[q_len - 2];
            if (diff == (seq_t)1) {
                inc_vals[inc_len++] = (seq_t)(k - 1); /* n = k‑1 */
            }
        }
    }

    return inc_vals[n - 1];
}
