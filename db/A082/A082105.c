/*
 * A082105  Array A(n, k) = (k*n)^2 + 4*(k*n) + 1, read by antidiagonals.
 * https://oeis.org/A082105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   A082105 read by antidiagonals. For a given index n (starting
 *            from 0), find the diagonal d (0‑based) such that d*(d+1)/2 ≤ n <
 *            (d+1)*(d+2)/2. Within that diagonal let p = n - d*(d+1)/2. Then
 *            the coordinates are i = p (row) and j = d - p (column). The
 *            value is (i*j)² + 4*(i*j) + 1
 * Range:     u8 n <= 38, u16 n <= 542, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* locate the diagonal d */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    /* number of elements before this diagonal */
    idx_t prev = d * (d + 1) / 2;

    /* position within the diagonal */
    idx_t p = n - prev;          /* 0 ≤ p ≤ d */

    /* coordinates (i, j) */
    idx_t i = p;
    idx_t j = d - p;

    /* compute (i*j)^2 + 4*(i*j) + 1 using seq_t arithmetic */
    seq_t ij = (seq_t)i * (seq_t)j;
    seq_t val = ij * ij + (seq_t)4 * ij + (seq_t)1;
    return val;
}
