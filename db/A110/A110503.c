/*
 * A110503  Triangle, read by rows, which shifts one column left under matrix
 *          inverse.
 * https://oeis.org/A110503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   The triangle T(n,k) is defined by T(n,k) = +1 when k is even,
 *            T(n,k) = -1 when k is odd, except T(k+2,k) = -2 when k is odd,
 *            and T(n,n) = 1 (diagonal override). The sequence a(n) reads this
 *            triangle row‑wise starting from row 0
 * Range:     i8 n <= 97, i16 n <= 97, i32 n <= 97, i64 n <= 97, i128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((__int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Starting index of this row. */
    idx_t start = ( (__int128)r * (r + 1) / 2 );
    idx_t c = n - start;          /* column index */

    /* Default value based on parity of column index. */
    seq_t val = ( (c & 1) ? (seq_t)-1 : (seq_t)1 );

    /* Override for the diagonal and the special -2 entry. */
    if (r == c)
        val = (seq_t)1;
    else if ( (c & 1) && (r == c + 2) )
        val = (seq_t)-2;

    return val;
}
