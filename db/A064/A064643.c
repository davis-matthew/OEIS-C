/*
 * A064643  Bidirectional 'Delannoy' variation of the Boustrophedon transform
 *          applied to all 1's sequence: Fill an triangular array in
 *          alternating directions. Let the first element of each row in that
 *          direction be equal to 1. Each next entry is given by T(n,k) =
 *          T(n,k +/- 1) + T(n-1,k-1) + T(n-1,k) + T(n-2,k...
 * https://oeis.org/A064643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064643 – Bidirectional “Delannoy” variation of the
   Boustrophedon transform applied to the all‑ones sequence.
   The triangular array is built row by row, each row filled in
   alternating left‑to‑right and right‑to‑left order.  The first
   element of every row (in the direction of filling) is 1, and every
   interior element satisfies
   T(n,k) = T(n,k-1) + T(n-1,k-1) + T(n-1,k) + T(n-2,k-1)   (left‑to‑right)
   or
   T(n,k) = T(n,k+1) + T(n-1,k-1) + T(n-1,k) + T(n-2,k-1)   (right‑to‑left).
   The final element of row n (after the bidirectional steps) is a(n).  */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

/* The types seq_t and idx_t are supplied by the test harness. */
extern seq_t a(idx_t n);

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* a(0) is specified as 1. */
    if (n == 0)
        return (seq_t)1;

    /* Row 0: only one element 1. */
    seq_t *prev = alloc_seq(1);
    if (!prev) return (seq_t)0;            /* allocation failure → 0 (unlikely). */
    prev[0] = (seq_t)1;
    size_t prev_len = 1;

    seq_t *prev2 = NULL;
    size_t prev2_len = 0;

    seq_t ret = 0;

    for (idx_t r = 1; r <= n; ++r) {
        size_t len = (size_t)r + 1;        /* current row length */
        seq_t *curr = alloc_seq(len);
        if (!curr) {                       /* allocation failure */
            free_seq(prev);
            free_seq(prev2);
            return (seq_t)0;
        }

        bool left_to_right = (r & 1u);      /* odd rows: left → right */

        if (left_to_right) {
            /* Fill from left to right. */
            curr[0] = (seq_t)1;
            for (size_t k = 1; k < len; ++k) {
                seq_t a1 = curr[k - 1];
                seq_t a2 = (k - 1 < prev_len) ? prev[k - 1] : (seq_t)0;
                seq_t a3 = (k < prev_len) ? prev[k] : (seq_t)0;
                seq_t a4 = (k - 1 < prev2_len) ? prev2[k - 1] : (seq_t)0;
                curr[k] = a1 + a2 + a3 + a4;
            }
            ret = curr[len - 1];
        } else {
            /* Fill from right to left. */
            curr[len - 1] = (seq_t)1;
            for (size_t k = len - 1; k-- > 0; ) {
                seq_t a1 = curr[k + 1];
                seq_t a2 = (k > 0 && k - 1 < prev_len) ? prev[k - 1] : (seq_t)0;
                seq_t a3 = (k < prev_len) ? prev[k] : (seq_t)0;
                seq_t a4 = (k > 0 && k - 1 < prev2_len) ? prev2[k - 1] : (seq_t)0;
                curr[k] = a1 + a2 + a3 + a4;
            }
            ret = curr[0];
        }

        /* Advance rows. */
        if (prev2 != NULL)
            free_seq(prev2);
        prev2 = prev;
        prev2_len = prev_len;
        prev = curr;
        prev_len = len;
    }

    /* Clean up the two most recent rows. */
    if (prev2 != NULL)
        free_seq(prev2);
    free_seq(prev);

    return ret;
}
