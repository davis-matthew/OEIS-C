/*
 * A021761  Decimal expansion of 1/757.
 * https://oeis.org/A021761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the (n+1)-th decimal digit of 1/757 (a(0)=0)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rem = 1; /* initial remainder for 1/757 */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10;                 /* bring down next digit */
        seq_t digit = rem / 757;    /* next decimal digit */
        rem = rem % 757;           /* update remainder */
        if (i == n)
            return digit;
    }
    /* Should never reach here */
    return 0;
}
