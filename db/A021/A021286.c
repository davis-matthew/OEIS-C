/*
 * A021286  Decimal expansion of 1/282.
 * https://oeis.org/A021286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (floor(10^(n+1) / 282)) mod 10
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        : decimal digit n of the expansion of 1/282 */

seq_t a(idx_t n)
{
    seq_t r = 1;       /* current remainder */
    seq_t digit = 0;   /* digit at current step */
    for (idx_t i = 0; i <= n; ++i) {
        r *= (seq_t)10;          /* bring next decimal digit into place */
        digit = r / (seq_t)282;  /* integer quotient is the decimal digit */
        r %= (seq_t)282;         /* remainder for next iteration */
    }
    return digit;
}
