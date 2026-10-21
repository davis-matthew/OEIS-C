/*
 * A063133  Dimension of the space of weight 2n cusp forms for Gamma_0( 65 ).
 * https://oeis.org/A063133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*n - 9 + (-1)^n for n ≥ 2, with a(1) = 5
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 5;
    /* a(0) does not exist in the sequence; return 0 for safety. */
    if (n == 0) return 0;
    seq_t nseq = (seq_t)n;
    seq_t res  = (seq_t)14 * nseq - (seq_t)9;
    /* add 1 if n is even, subtract 1 if n is odd. */
    if ((n & 1) == 0)
        res += (seq_t)1;
    else
        res -= (seq_t)1;
    return res;
}
