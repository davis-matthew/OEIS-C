/*
 * A021633  Decimal expansion of 1/629.
 * https://oeis.org/A021633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (floor(10^(n+1) / 629)) mod 10 – the n‑th decimal digit
 *            of 1/629 (offset 0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t remainder = 1;            /* start with 1/629 remainder 1 */
    for (idx_t i = 0; i <= n; ++i)
    {
        idx_t tmp   = remainder * 10;          /* shift one decimal place */
        seq_t digit = (seq_t)(tmp / 629);      /* next decimal digit */
        remainder = tmp % 629;                 /* update remainder */
        if (i == n)
            return digit;
    }
    return 0;    /* unreachable for n >= 0 */
}
