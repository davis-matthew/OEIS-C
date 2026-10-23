/*
 * A021597  Decimal expansion of 1/593.
 * https://oeis.org/A021597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth decimal digit of 1/593
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const uint64_t denom = 593ULL;
    uint64_t rem = 1;          /* start with numerator 1 */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;          /* bring next decimal place into remainder */
        digit = (seq_t)(rem / denom);  /* the digit at this position */
        rem %= denom;          /* new remainder for the next step */
    }
    return digit;
}
