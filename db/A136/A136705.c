/*
 * A136705  Triangle read by rows where the n-th row gives the coefficients of
 *          the characteristic polynomial for a Fibonacci-type matrix with a=1
 *          and b=1.
 * https://oeis.org/A136705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, easy, sign
 * Formula:   a(n) = coefficient {0‑based} from the characteristic–polynomial
 *            triangle of the Fibonacci‑type matrix. The computation uses the
 *            triangular index representation: let r be the unique row such
 *            that T_r = r*(r+1)/2 ≤ n < T_{r+1}. Then let offset = n - T_r.
 *            For r = 0 return 1. For r = 1 return (offset...
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that T_r = r*(r+1)/2 ≤ n < T_{r+1}.  */
    idx_t r   = 0;
    unsigned __int128 tr = 0;          /* triangular number T_r, 128‑bit protects overflow */

    while (tr + r + 1 <= n) {
        ++r;
        tr += r;                       /* tr == r*(r+1)/2 after the increment */
    }

    /* offset within the row (0‑based) */
    idx_t offset = (idx_t)(n - (idx_t)tr);

    if (r == 0)                       /* row 0: [1] */
        return (seq_t)1;

    if (r == 1) {                     /* row 1: [1, -1] */
        return (offset == 0) ? (seq_t)1 : (seq_t)-1;
    }

    /* r >= 2 */
    /* sign for (-1)^(r+1)  and (-1)^r  */
    seq_t sign1 = ((r + 1) & 1) ? (seq_t)-1 : (seq_t)1;  /* (-1)^(r+1) */
    seq_t sign2 = (r & 1) ? (seq_t)-1 : (seq_t)1;         /* (-1)^r     */

    if (offset == 0 || offset == r - 1)
        return sign1;
    if (offset == r)
        return sign2;
    return (seq_t)0;                  /* all other positions are zero */
}
