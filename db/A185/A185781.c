/*
 * A185781  Accumulation array of A185780, by antidiagonals.
 * https://oeis.org/A185781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the antidiagonal coordinates of
 *            index n (i = 1..d, j = d..1, d minimal with d(d+1)/2 ≥ n)
 * Range:     u8 n <= 23, u16 n <= 307, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the value of sequence A185781, the accumulation array of A185780
   by antidiagonals. */
seq_t a(idx_t n)
{
    /* Handle offset 1: a(1) is the first term. */
    if (!n)
        return (seq_t)0;

    /* Find diagonal d such that d(d+1)/2 >= n. */
    uint64_t d = 1;
    uint64_t tri = 1;          /* current triangular number d(d+1)/2 */
    while (tri < n)
    {
        ++d;
        tri = d * (d + 1) / 2;
    }

    /* Count of elements before this diagonal. */
    uint64_t prev = (d - 1) * d / 2;

    /* Zero–based index within the diagonal. */
    uint64_t offset = n - prev - 1;

    /* Coordinates (row i, column j). */
    uint64_t i = offset + 1;           /* row 1..d */
    uint64_t j = d - offset;           /* column d..1 */

    /* Compute T(i,j) = j*(j+1)*i*(j*i - i + j + 2)/6 using seq_t arithmetic */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t term = jj * (jj + 1) * ii *
                 (jj * ii - ii + jj + 2) / 6;

    return term;
}
