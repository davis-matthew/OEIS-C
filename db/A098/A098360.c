/*
 * A098360  Multiplication table of the cube numbers read by antidiagonals.
 * https://oeis.org/A098360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (i*j)^3 where (i,j) are the coordinates obtained from
 *            reading the multiplication table of the positive cube numbers
 *            {k^3} along the antidiagonals. The index (i,j) is found by
 *            locating the nth term in the nth antidiagonal of the table
 * Range:     u8 n <= 11, u16 n <= 71, u32 n <= 3196, u64 n <= 8910,
 *            u128 n <= 8910, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal number d such that d*(d+1)/2 >= n. */
    idx_t low = 1, high = n, d = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n) {
            d = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    /* offset inside the diagonal (0‑based). */
    idx_t prev   = (d - 1) * d / 2;
    idx_t offset = n - prev - 1;

    /* Coordinates in the multiplication table. */
    idx_t i = offset + 1;
    idx_t j = d - offset;

    /* Compute (i*j)^3 in the seq_t domain. */
    seq_t prod = (seq_t)i * (seq_t)j;
    return prod * prod * prod;
}
