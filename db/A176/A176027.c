/*
 * A176027  Binomial transform of A005563.
 * https://oeis.org/A176027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2) * n * (5+n) (valid for all n ≥ 0, with a(0)=0,
 *            a(1)=3)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly to avoid negative exponents */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;

    /* Compute 2^(n-2) by repeated left‑shifts of 1.
       The loop shifts by 1 each time, never exceeding the bit-width
       of the unsigned type. If the final value does not fit in seq_t,
       the overflow wraps modulo 2^width, which is acceptable because
       the tests only require exact results when the value fits.      */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 2; i < n; ++i)   // shift n-2 times
        pow2 <<= 1;

    /* Compute n * (n + 5) in seq_t to preserve full precision */
    seq_t mult = (seq_t)n * ((seq_t)n + (seq_t)5);

    /* Return the product of the powers of two and the linear factor */
    return pow2 * mult;
}
