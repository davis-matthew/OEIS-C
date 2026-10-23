/*
 * A048147  Array T read by diagonals; T(i,j) = i^2 + j^2.
 * https://oeis.org/A048147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = i^2 + j^2, where (i,j) are the coordinates of the n-th
 *            element obtained by reading the array T(i,j)=i^2+j^2 along its
 *            antidiagonals (starting from (0,0), then (0,1),(1,0), then
 *            (0,2),(1,1),(2,0), etc.)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find diagonal d such that T(d)=d*(d+1)/2 <= n < T(d+1). */
    idx_t d = 0;          /* current diagonal index */
    idx_t t = 0;          /* triangular number T(d) for current d */

    while (t + d + 1 <= n) {
        d++;          /* move to next diagonal */
        t += d;       /* update triangular number: T(d) = T(d-1) + d */
    }

    /* Position within the diagonal. */
    idx_t k = n - t;     /* 0-based index in diagonal d */

    /* Coordinates on the diagonal: i grows from 0 to d. */
    seq_t i = (seq_t)k;
    seq_t j = (seq_t)(d - k);

    /* Return the value in the array. */
    return i * i + j * j;
}
