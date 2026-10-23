/*
 * A192062  Square Array T(ij) read by antidiagonals (from NE to SW) with
 *          columns 2j being the denominators of continued fraction
 *          convergents to square root of (j^2 + 2j).
 * https://oeis.org/A192062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* AOJ: Compute OEIS sequence A192062.
   The sequence is obtained from the square array T(i,j) defined by

   T(0,j) = 0,
   T(1,j) = 1,
   T(2k,j)   = T(2k-2,j) + T(2k-1,j)               (k ≥ 1),
   T(2k+1,j) = T(2k-1,j) + j * T(2k,j)             (k ≥ 1).

   The sequence is the antidiagonal read of this array (NE → SW).
   For an index n (0‑based) let d be the smallest integer with
     (d+1)(d+2)/2 > n.
   Then
     i = n - d(d+1)/2   (row index)
     j = d - i          (column index)

   and a(n) = T(i,j). */

#include <stdint.h>

/* Compute one entry of the array.  The loop never overflows idx_t or
   seq_t because n is small (≤ 10000) and sequence growth is moderate. */
static seq_t T_value(idx_t i, idx_t j)
{
    if (i == 0)             return (seq_t)0;
    if (i == 1)             return (seq_t)1;

    seq_t t_prev2 = (seq_t)0;   /* T(i-2, j) */
    seq_t t_prev1 = (seq_t)1;   /* T(i-1, j) */
    seq_t t_curr  = 0;

    for (idx_t k = 2; k <= i; ++k)
    {
        if ((k & 1) == 0)     /* even k: T(2k,j) = T(2k-2,j) + T(2k-1,j) */
            t_curr = t_prev2 + t_prev1;
        else                  /* odd k:  T(2k+1,j) = T(2k-1,j) + j*T(2k,j) */
            t_curr = t_prev2 + (seq_t)j * t_prev1;

        t_prev2 = t_prev1;
        t_prev1 = t_curr;
    }
    return t_curr;
}

seq_t a(idx_t n)
{
    /* Find the diagonal d that contains index n. */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n)
        ++d;

    /* Compute i and j from n. */
    idx_t cumulative_prev = (d * (d + 1)) / 2;   /* sum of lengths of previous diagonals */
    idx_t i = n - cumulative_prev;
    idx_t j = d - i;

    return T_value(i, j);
}
