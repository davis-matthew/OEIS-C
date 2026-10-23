/*
 * A021491  Decimal expansion of 1/487.
 * https://oeis.org/A021491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the (n+1)-th digit after the decimal point in the decimal
 *            expansion of 1/487
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the remainder after each decimal multiplication; always < 487 */
    unsigned long long rem = 1ULL;
    unsigned long long digit = 0ULL;
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;           /* shift left by one decimal place */
        digit = rem / 487ULL;   /* next digit */
        rem %= 487ULL;          /* new remainder */
    }
    return (seq_t)digit;
}
