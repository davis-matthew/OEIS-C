/*
 * A028297  Coefficients of Chebyshev polynomials of the first kind: triangle
 *          of coefficients in expansion of cos(n*x) in descending powers of
 *          cos(x).
 * https://oeis.org/A028297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, easy, sign
 * Formula:   a(n) is the n‑th term of the flattened triangle of coefficients
 *            * of the Chebyshev polynomials of the first kind, i.e. *
 *            T_0(x)=1, T_1(x)=x, * T_{m+1}(x)=2x·T_m(x)-T_{m-1}(x). * Row m
 *            contains the coefficients of T_m(x) ordered by descending powers
 *            * of x, with length ⌊m/2⌋+1. The sequence a(n) i...
 * Range:     i8 n <= 19, i16 n <= 57, i32 n <= 198, i64 n <= 734,
 *            i128 n <= 2718, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* number of coefficients in row m */
static inline idx_t row_len(idx_t m)
{
    return (m >> 1) + 1;          /* floor(m/2)+1 */
}

/* public function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* locate the row r and column k that contain the n‑th flattened entry */
    idx_t cum = 0;                /* number of terms before row r                */
    idx_t r   = 0;

    for ( ; ; ++r) {
        idx_t rl = row_len(r);
        if (cum + rl > n) break;  /* n lies in this row */
        cum += rl;
    }
    idx_t k = n - cum;            /* column inside row r   */

    /* trivial rows */
    if (r <= 1) return (seq_t)1;

    /* allocate first two rows (T0 and T1) */
    seq_t *row_prevprev = alloc_seq_array(row_len(0));
    row_prevprev[0] = (seq_t)1;                    /* T0 : 1 */

    seq_t *row_prev = alloc_seq_array(row_len(1));
    row_prev[0] = (seq_t)1;                        /* T1 : x */

    idx_t len_prevprev = row_len(0);
    idx_t len_prev     = row_len(1);
    seq_t *row_cur     = NULL;
    idx_t len_cur      = 0;

    /* build rows up to r, keeping only the last two rows */
    for (idx_t m = 2; m <= r; ++m) {
        len_cur = row_len(m);
        row_cur = alloc_seq_array(len_cur);

        for (idx_t i = 0; i < len_cur; ++i) {
            seq_t val = 0;

            /* contribution from 2·x·T_{m-1}  →  2·row_prev[i]   */
            if (i < len_prev)
                val += (seq_t)2 * row_prev[i];

            /* contribution from –T_{m-2}  →  –row_prevprev[i-1] */
            if (i > 0 && (i-1) < len_prevprev)
                val -= row_prevprev[i-1];

            row_cur[i] = val;
        }

        /* recycle memory: dispose T_{m-2} */
        free_seq_array(row_prevprev);
        row_prevprev = row_prev; len_prevprev = len_prev;

        /* advance: T_{m-1} becomes row_prev, T_m becomes row_prev */
        row_prev = row_cur;       len_prev = len_cur;
    }

    /* row_prev now holds T_r(x) */
    seq_t result = row_prev[k];

    /* clean up the two remaining rows */
    free_seq_array(row_prevprev);
    free_seq_array(row_prev);

    return result;
}
