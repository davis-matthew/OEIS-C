/*
 * A021835  Decimal expansion of 1/831.
 * https://oeis.org/A021835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth decimal digit of 1/831 (after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t denom = 831;
    idx_t r = 1;          /* current remainder */
    seq_t digit = 0U;     /* digit at the current position */

    for (idx_t i = 0; i <= n; ++i) {
        r *= (idx_t)10;
        digit = (seq_t)(r / denom);
        r %= denom;
    }
    return digit;
}
