/*
 * A021899  Decimal expansion of 1/895.
 * https://oeis.org/A021899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/895) mod 10 (digit of 1/895)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t denom = 895;
    idx_t r = 1;
    idx_t i;
    for (i = 0; i <= n; ++i) {
        r *= 10;
        seq_t digit = (seq_t)(r / denom);
        r %= denom;
        if (i == n) return digit;
    }
    return 0; /* unreachable */
}
