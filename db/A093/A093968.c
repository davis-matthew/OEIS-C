/*
 * A093968  Inverse binomial transform of n*Pell(n).
 * https://oeis.org/A093968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * 2^floor((n-1)/2) for n ≥ 1, a(0) = 0
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* exponent of 2: floor((n-1)/2) */
    idx_t shift = (n - 1) / 2;

    /* compute 2^shift by repeated doubling.  Doubling on unsigned integer types
     * is defined to wrap modulo the type width, which is fine for values that
     * will overflow the 64‑bit build; the tests only check terms that fit. */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < shift; ++i)
        pow2 *= 2;

    return ((seq_t)n) * pow2;
}
