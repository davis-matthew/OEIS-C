/*
 * A143595  Triangle read by rows, A000012 * (an infinite lower triangular
 *          matrix with 1's in the first column and the rest 2's); 1<=k<=n.
 * https://oeis.org/A143595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the unique integer such that r(r-1)/2 < n ≤ r(r+1)/2
 *            (i.e., r is the row number of the triangle). Let p = r(r-1)/2 be
 *            the total number of terms before row r, t = n - p - 1 the
 *            0‑based position inside that row. Then the n‑th term is r if t
 *            == 0 2*(r - t) otherwise (Row r of the triangle i...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r.  Since n ≤ 10000 in the intended tests,
       a simple linear search is fast enough and avoids any floating‑point
       or external sqrt routine.  r never exceeds about 150. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Number of terms preceding row r. */
    idx_t prev = (r * (r - 1)) / 2;

    /* Position inside the row, 0‑based. */
    idx_t t = n - prev - 1;

    if (t == 0)
        return (seq_t)r;
    else
        return (seq_t)(2 * (r - t));
}
