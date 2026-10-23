/*
 * A128307  Triangle, (1, 0, 1, 2, 4, 8, ...) in every column.
 * https://oeis.org/A128307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry in the triangular array defined by T(r,c) =
 *            g_{r-c}, where g_0 = 1, g_1 = 0, and g_m = 2^{m-2} for m ≥ 2.
 *            The sequence is generated in row-major order with row r
 *            containing r entries (c from 1 to r). The index n (offset 1) is
 *            mapped to the smallest r such that r(r+1)/2 ≥ n, then...
 * Range:     u8 n <= 55, u16 n <= 171, u32 n <= 595, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed */

static seq_t g_value(idx_t m)
{
    if (m == 0) return 1;
    if (m == 1) return 0;
    seq_t v = 1;
    for (idx_t i = 0; i < m - 2; ++i)
        v *= 2;   /* multiply by 2 m-2 times ⇒ 2^{m-2}     */
    return v;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n using a simple loop.
       For n up to 10,000 this takes at most ~141 iterations. */
    idx_t r = 1;
    idx_t tri = 1;          /* triangular number r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;
    }

    /* previous total terms before row r */
    idx_t prev = tri - r;
    /* column index (1-based) within the row */
    idx_t c = n - prev;
    /* index m = r - c for g_{m} */
    idx_t m = r - c;

    return g_value(m);
}
