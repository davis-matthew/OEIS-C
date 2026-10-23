/*
 * A021088  Decimal expansion of 1/84.
 * https://oeis.org/A021088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n‑th digit of the decimal expansion of 1/84. a(0) is
 *            the integer part (0), a(1) and a(2) are the first two digits
 *            after the leading zero of the fractional part. For n >= 3 the
 *            digits repeat in the fixed period 9,0,4,7,6,1
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n < 3)        /* a(1) = a(2) = 1 */
        return 1;
    static const seq_t period[6] = {9, 0, 4, 7, 6, 1};
    return period[(n - 3) % 6];
}
