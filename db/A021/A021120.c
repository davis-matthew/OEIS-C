/*
 * A021120  Decimal expansion of 1/116.
 * https://oeis.org/A021120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth decimal digit of 1/116 computed by long division
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined externally by the build system. */
seq_t a(idx_t n)
{
    idx_t i;
    seq_t digit = 0;
    unsigned int rem = 1;          /* remainder < 116 */

    for (i = 0; i <= n; ++i) {
        rem *= 10;                 /* bring down next digit */
        digit = rem / 116;         /* determine current decimal digit */
        rem = rem % 116;           /* update remainder for next step */
    }
    return digit;
}
