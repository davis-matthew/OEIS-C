/*
 * A021700  Decimal expansion of 1/696.
 * https://oeis.org/A021700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/696) mod 10, i.e. the nth decimal digit of
 *            1/696
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t   r = 1;          /* current remainder of 1 ÷ 696, 0 ≤ r < 696 */
    seq_t   digit = 0;      /* digit at the current position */

    for (idx_t i = 0; i <= n; ++i) {
        r = r * 10;                     /* bring down next zero */
        digit = (seq_t)(r / 696);       /* next decimal digit */
        r = r % 696;                    /* new remainder */
    }
    return digit;
}
