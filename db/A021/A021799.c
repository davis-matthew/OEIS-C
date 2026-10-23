/*
 * A021799  Decimal expansion of 1/795.
 * https://oeis.org/A021799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th digit after the decimal point of 1/795 * i.e.
 *            a(n) is the digit at index n in the decimal expansion * of the
 *            rational number 1/795
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* remainder of the division 1 ÷ 795 after each decimal digit */
    seq_t r = 1;
    seq_t digit = 0;               /* holds the current digit (0‑9) */

    /* Perform long division n+1 times, obtaining the (n+1)-th
       digit (indexed from 0). */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t tmp = r * 10;        /* multiply the rest by base 10 */
        digit = tmp / 795;         /* integer part is the next digit */
        r = tmp % 795;             /* remainder for the next step     */
    }
    return digit;
}
