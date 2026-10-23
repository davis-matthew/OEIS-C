/*
 * A106472  Expansion of g.f. (1 - x)^2*(1 + x) / (1 - 2*x)^2.
 * https://oeis.org/A106472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3; for n ≥ 2: a(n) = (3*n + 8)*2^(n-3) (the
 *            exponent is non‑negative for n≥3)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the two small indices */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* n == 2 is the only case where the exponent would be -1;
       the value is 7 by definition, so special‑case it. */
    if (n == 2) return (seq_t)7;

    /* compute (3*n + 8)  */
    seq_t base = (seq_t)3 * (seq_t)n + (seq_t)8;

    /* 2^(n-3) is a left shift of 1 by (n-3) bits.
       The shift count is non‑negative here (n ≥ 3).   */
    seq_t pow = ((seq_t)1) << (n - 3);

    return base * pow;
}
