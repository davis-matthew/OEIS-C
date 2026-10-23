/*
 * A021135  Decimal expansion of 1/131.
 * https://oeis.org/A021135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit after the decimal point in the expansion of
 *            1/131, offset 0 (a(0) is the first digit)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;           /* current remainder (starts with 1/131 base). */
    seq_t d = 0;           /* digit to return. */
    for (idx_t i = 0; i <= n; ++i) {
        r = r * (seq_t)10;
        d = r / (seq_t)131;
        r = r % (seq_t)131;
    }
    return d;
}
