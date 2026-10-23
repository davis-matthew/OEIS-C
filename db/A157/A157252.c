/*
 * A157252  Denominator of Euler(n, 1/29).
 * https://oeis.org/A157252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 29^n * 2^{v2(n+1)} (v2 is the exponent of 2 dividing n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;

    /* compute 29^n */
    for (idx_t i = 0; i < n; ++i) {
        res *= (seq_t)29;
    }

    /* compute v2(n+1) */
    idx_t m = n + 1;
    unsigned int exp = 0;
    while ((m & 1) == 0) {
        ++exp;
        m >>= 1;
    }

    /* multiply by 2^exp */
    res <<= exp;

    return res;
}
