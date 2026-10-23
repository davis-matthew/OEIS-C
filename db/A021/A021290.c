/*
 * A021290  Decimal expansion of 1/286.
 * https://oeis.org/A021290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(((10^n) mod 286) * 10 / 286)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long rem = 1ULL;
    for (idx_t i = 0; i < n; ++i) {
        rem = (rem * 10ULL) % 286ULL;
    }
    return (seq_t)((rem * 10ULL) / 286ULL);
}
