/*
 * A021863  Decimal expansion of 1/859.
 * https://oeis.org/A021863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th decimal digit of 1/859 (with a(0) being the
 *            first digit after the point)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0 <= n is assumed (behaviour for n < 0 is undefined for unsigned idx_t) */
    idx_t r = 1;          /* remainder after the fractional part has been extracted so far */
    seq_t digit = 0;      /* the current digit of the expansion */

    /* Iterate through the first n+1 digits, because we need the digit at index n */
    for (idx_t i = 0; i <= n; ++i) {
        r = r * 10;                    /* shift remainder left by one decimal place */
        digit = (seq_t)(r / 859);      /* extract the next decimal digit 0-9 */
        r = r % 859;                   /* keep only the new remainder */
    }

    return digit;
}
