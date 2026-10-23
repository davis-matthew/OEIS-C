/*
 * A021868  Decimal expansion of 1/864.
 * https://oeis.org/A021868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the (n+1)-th digit of the decimal expansion of 1/864,
 *            computed by long division
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t rem = 1;          /* current remainder of 1 divided by 864 */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                    /* bring down next zero */
        digit = rem / 864;            /* next decimal digit */
        rem   = rem % 864;            /* update remainder */
    }
    return digit;
}
