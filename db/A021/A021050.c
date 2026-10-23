/*
 * A021050  Decimal expansion of 1/46.
 * https://oeis.org/A021050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0) = 0; for n >= 1, a(n) = floor(10^(n+1)/46) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digits are obtained by long division of 1 by 46, skipping the
   integer part 0 and the leading fractional digit 0. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* integer part of 1/46 */

    idx_t r = 1;                          /* current remainder */
    r = (10 * r) % 46;                    /* skip the first fractional digit */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit = (seq_t)(10 * r / 46);/* next digit of the expansion   */
        r = (10 * r) % 46;                /* update remainder              */
        if (i == n) return digit;         /* return the requested digit    */
    }

    return 0; /* unreachable, satisfies compiler */
}
