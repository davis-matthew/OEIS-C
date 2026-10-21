/*
 * A219768  Number of n X 3 arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          columns, 0..1 n X 3 array.
 * https://oeis.org/A219768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 6*n^3 - 25*n^2 + 138*n - 96) / 24 for n > 1; a(1)
 *            = 3
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)3;

    seq_t nseq = n;                 /* promote index to seq_t */
    seq_t n2   = nseq * nseq;       /* n^2 */
    seq_t n3   = n2 * nseq;         /* n^3 */
    seq_t n4   = n3 * nseq;         /* n^4 */

    /* Compute S = n^4 + 6*n^3 + 138*n - 96   (positive enough) */
    seq_t s = n4 + 6 * n3 + 138 * nseq - 96;

    /* Subtract 25*n^2 */
    s -= 25 * n2;

    /* Divide by 24 to get the final result */
    return s / 24;
}
