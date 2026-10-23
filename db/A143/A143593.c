/*
 * A143593  Triangle read by rows, square of A238303 (an infinite lower
 *          triangular matrix with 1's in the first column and the rest 2's).
 * https://oeis.org/A143593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) – Triangle read by rows, square of A238303 (first column 1, rest 2).
 *  The lower triangular matrix L has L[i][j] = 1 if j==1 else 2 for j≤i.
 *  Its square S = L² satisfies
 *      S[i][1] = 2*i - 1                       (first column)
 *      S[i][j] = 4*(i - j + 1)  for j>1       (rest of the row)
 *  The sequence is read row‑wise, so a(1)=S[1][1], a(2)=S[2][1], a(3)=S[2][2], …
 *
 *  For the input linear index n (1‑based) we locate the row r and
 *  column offset j (0‑based) by binary search on the triangular numbers.
 *  Then the value is computed directly from the formulas above.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n starts at 1 – for safety handle n==0 */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* 0‑based linear index */
    idx_t t = n - (idx_t)1;

    /* Find the smallest r such that r*(r+1)/2 > t.
     * r will be 1‑based row index.  Use 128‑bit arithmetic to avoid overflow. */
    idx_t low = 0, high = 1;
    while (((__uint128_t)high * (high + (idx_t)1) / 2) <= t)
        high <<= 1;

    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__uint128_t)mid * (mid + (idx_t)1) / 2) <= t)
            low = mid;
        else
            high = mid;
    }

    idx_t r   = high;                     /* 1‑based row number   */
    idx_t prev = ( (__uint128_t)low * (low + (idx_t)1) / 2 ); /* elements before row r */
    idx_t j   = t - prev;                 /* 0‑based column offset */

    if (j == (idx_t)0) {
        /* first column */
        return (seq_t)( (seq_t)2 * (seq_t)r - (seq_t)1 );
    } else {
        /* other columns */
        return (seq_t)( (seq_t)4 * (seq_t)(r - j) );
    }
}
