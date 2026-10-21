/*
 * A093033  Number of interior balls in a truncated octahedral arrangement.
 * https://oeis.org/A093033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^3 - 15*n^2 + 6*n - 1
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 645, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence is defined only for n ≥ 1, return 0 otherwise */
    if (n < 1) return 0;

    /* Promote the index to the sequence type */
    seq_t nn = (seq_t)n;

    /* Compute n^2, n^3 and the polynomial value */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t res = 16 * n3 - 15 * n2 + 6 * nn - 1;

    return res;
}
