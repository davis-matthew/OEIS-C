/*
 * A021100  Decimal expansion of 1/96.
 * https://oeis.org/A021100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   decimal digit of 1/96 at offset n (0‑based). 1/96 =
 *            0.010416666666… ; a(n) = 6 for n ≥ 5
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t remainder = 1;   /* start with p = 1 (numerator of 1/96)   */
    seq_t digit    = 0;
    idx_t i;

    for (i = 0; i <= n; ++i) {
        remainder *= 10;          /* multiply by base 10          */
        digit = remainder / 96;   /* next decimal digit           */
        remainder = remainder % 96;/* new remainder for next step   */
    }

    return digit;
}
