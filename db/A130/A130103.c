/*
 * A130103  Expansion of e.g.f. e^(2x)-(1+x)*e^x+x.
 * https://oeis.org/A130103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1, and for n≥2 a(n) = 2^n – n – 1 The sequence
 *            satisfies a(n) = Sum_{i=1}^{n-1} i*2^(n-i-1) (n≥2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) pow2 *= 2;   // compute 2^n

    pow2 -= (seq_t)n;
    pow2 -= 1;

    return pow2;
}
