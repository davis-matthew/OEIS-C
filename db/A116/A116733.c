/*
 * A116733  Number of permutations of length n which avoid the patterns 321,
 *          1324, 2341.
 * https://oeis.org/A116733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n=1, 2 for n=2; for n>=3: a(n) = (5*n^2 - 21*n +
 *            28)/2
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)2;
    }
    /* Compute using the explicit quadratic formula */
    seq_t nn = (seq_t)n;
    seq_t res = ((seq_t)5 * nn * nn - (seq_t)21 * nn + (seq_t)28) / (seq_t)2;
    return res;
}
