/*
 * A021785  Decimal expansion of 1/781.
 * https://oeis.org/A021785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit after the decimal point in the decimal
 *            expansion of 1/781. The digit is obtained by performing long
 *            division: remainder *= 10; digit = remainder / 781; remainder %=
 *            781; repeating for n+1 steps
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long rem = 1ULL;  /* current remainder (0 <= rem < 781) */
    seq_t digit = 0;                /* last digit produced */

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;                               /* shift left for next digit */
        digit = (seq_t)(rem / 781ULL);              /* integer quotient = next digit */
        rem   = rem % 781ULL;                       /* new remainder for next step */
    }
    return digit;
}
