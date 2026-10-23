/*
 * A036217  Expansion of 1/(1-3*x)^5; 5-fold convolution of A000244 (powers of
 *          3).
 * https://oeis.org/A036217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n * binomial(n+4, 4)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient (n+4 choose 4) */
    seq_t num = 1;

    num *= (seq_t)(n + 1);
    num *= (seq_t)(n + 2);
    num *= (seq_t)(n + 3);
    num *= (seq_t)(n + 4);

    num /= (seq_t)24;  /* exact division */

    /* Compute 3^n by repeated multiplication */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= (seq_t)3;

    return num * pow3;
}
