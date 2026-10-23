/*
 * A144329  Triangle read by rows, A000012 * A144328.
 * https://oeis.org/A144329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The triangle A144329 has entries T(r,c) = r for c = 1, T(r,c) =
 *            (r - c + 1) * (c - 1) for c ≥ 2. These entries are listed
 *            row–major to give the sequence a(n)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find the row r such that n ≤ r(r+1)/2 */
    idx_t r = 1;
    idx_t cumulative = 1;          /* cumulative elements up to row r   */
    while (cumulative < n)
    {
        ++r;
        cumulative += r;
    }

    /* compute the column index c (1‑based) */
    idx_t prev_cumulative = cumulative - r;   /* elements before row r */
    idx_t c = n - prev_cumulative;            /* column in row r       */

    /* compute the triangle value */
    if (c == 1)
        return (seq_t)r;
    else
        return (seq_t)(r - c + 1) * (seq_t)(c - 1);
}
