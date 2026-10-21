/*
 * A066810  Expansion of x^2/((1-3*x)*(1-2*x)^2).
 * https://oeis.org/A066810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2^n - n*2^(n-1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 3^n and 2^n */
    seq_t pow3 = 1;   /* 3^0 */
    seq_t pow2 = 1;   /* 2^0 */
    for (idx_t i = 1; i <= n; ++i) {
        pow3 *= 3;
        pow2 *= 2;
    }

    /* 2^(n-1) is pow2 divided by 2 */
    seq_t pow2_n_minus_1 = pow2 / 2;

    /* a(n) = 3^n - 2^n - n*2^(n-1) */
    return pow3 - pow2 - (seq_t)n * pow2_n_minus_1;
}
