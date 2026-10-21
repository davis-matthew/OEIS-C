/*
 * A051375  Number of Boolean functions of n variables and rank 3 from Post
 *          class F(5,inf).
 * https://oeis.org/A051375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4^n - 3^n - 3*2^n + 5)/2 (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n, 3^n and 4^n by simple iteration */
    seq_t pow2 = 1;
    seq_t pow3 = 1;
    seq_t pow4 = 1;

    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
        pow3 *= (seq_t)3;
        pow4 *= (seq_t)4;
    }

    /* Apply the formula */
    seq_t term = pow4 - pow3 - ( (seq_t)3 * pow2 ) + (seq_t)5;
    return term / (seq_t)2;
}
