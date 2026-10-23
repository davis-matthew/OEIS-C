/*
 * A133128  Triangle of first differences of A120070 with a leftmost column of
 *          A002522.
 * https://oeis.org/A133128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less, easy
 * Formula:   a(n) = if n is a triangular number T_r = r*(r+1)/2, then a(n) =
 *            r^2 + 2r + 2 (left‑most element of row r), otherwise a(n) =
 *            -(2*(n - T_r) + 1) (remaining elements of row r)
 * Range:     i8 n <= 65, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find row r such that T_r <= n < T_{r+1}
       and t = T_r = r*(r+1)/2                      */
    idx_t r = 0;
    idx_t t = 0;               /* current triangular number */
    while (t + r + 1 <= n) {   /* next triangular number would be t+(r+1) */
        r++;
        t += r;
    }
    idx_t j = n - t;           /* position in the row (0‑based) */

    if (j == 0) {              /* leftmost element of the row */
        seq_t rr = (seq_t)r;   /* safe cast; rr fits in seq_t for required n */
        return rr * rr + 2 * rr + 2;
    } else {                   /* remaining elements */
        return -(((seq_t)(2 * j)) + 1);
    }
}
