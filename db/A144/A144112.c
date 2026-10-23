/*
 * A144112  Weight array W={w(i,j)} of the natural number array A000027.
 * https://oeis.org/A144112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = weight at antidiagonal coordinates (i,j) of the matrix *
 *            W where * W(1,1) = 1 * W(1,j) = j-1 (j > 1) * W(i,1) = i (i > 1)
 *            * W(i,j) = 1 (i > 1 and j > 1) * * The sequence is obtained by
 *            reading W along diagonals with constant i+j, * enumerated as
 *            follows: * s = 2,3,4,... * for each s list t...
 * Range:     u8 n <= 9645, u16 n <= 9645, u32 n <= 9645, u64 n <= 9645,
 *            u128 n <= 9645, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against caller errors; the OEIS offset is 1. */
    if (n == 0) return 0;

    /* Find the diagonal s such that s*(s-1)/2 >= n. */
    idx_t s = 2;
    while ((unsigned __int128)s * (s - 1) / 2 < n)
        ++s;

    /* Number of terms preceding this diagonal. */
    idx_t prev = s - 1;
    idx_t terms_before = (unsigned __int128)prev * (prev - 1) / 2;

    /* Position within diagonal (1 ≤ k ≤ s-1). */
    idx_t k = n - terms_before;      /* 1 ≤ k ≤ s-1 */
    idx_t i = k;                     /* row index          */
    idx_t j = s - k;                 /* column index       */

    /* Compute the weight w(i,j). */
    seq_t val;
    if (i == 1 && j == 1) {
        val = 1;
    } else if (i == 1) {
        val = (seq_t)(j - 1);
    } else if (j == 1) {
        val = (seq_t)i;
    } else {
        val = 1;
    }

    return val;
}
