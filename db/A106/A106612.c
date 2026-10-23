/*
 * A106612  a(n) = numerator(n/(n+11)).
 * https://oeis.org/A106612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, mult, easy
 * Formula:   a(n) = n/gcd(n,11) = n if (n % 11 ≠ 0) else n/11
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n % 11ULL) == 0ULL)
        return (seq_t)(n / 11ULL);
    else
        return (seq_t)n;
}
