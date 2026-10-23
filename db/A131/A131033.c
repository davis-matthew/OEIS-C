/*
 * A131033  A130296 * A097806.
 * https://oeis.org/A131033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened entries of the lower‑triangular matrix M =
 *            A130296 * A097806. The matrix M has the form M(1,1) = 1 M(i,1) =
 *            i+1 (i > 1) M(i,j) = 2 (i > 1, 2 ≤ j ≤ i-1) M(i,i) = 1 (i > 1)
 *            a(n) enumerates M row‑by‑row: a(1)=1, a(2)=3, a(3)=1, a(4)=4,
 *            etc
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n.  */
    idx_t r = 0;
    while (((unsigned __int128)r * (r + 1)) / 2 < n)
        ++r;

    /* Number of elements before row r. */
    idx_t prev = ((unsigned __int128)(r - 1) * r) / 2;

    /* j is the position within the row (1‑based). */
    idx_t j = n - prev;          // 1 <= j <= r

    seq_t val;
    if (r == 1) {
        val = 1;                 /* only element in row 1 */
    } else if (j == 1) {
        val = (seq_t)(r + 1);    /* first element of row r */
    } else if (j == r) {
        val = 1;                 /* diagonal element */
    } else {
        val = 2;                 /* all other elements in row r */
    }
    return val;
}
