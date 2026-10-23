/*
 * A021931  Decimal expansion of 1/927.
 * https://oeis.org/A021931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = ⌊ (10^(n+1) / 927) ⌋ mod 10 * i.e. the n‑th digit after
 *            the decimal point in 1/927
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t denom = 927;
    idx_t rem = 1 % denom;   /* remainder after 10^0 */
    for (idx_t i = 0; i < n; ++i) {
        rem = (rem * 10) % denom;
    }
    idx_t digit = (rem * 10) / denom;
    return (seq_t)digit;
}
