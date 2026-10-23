/*
 * A021639  Decimal expansion of 1/635.
 * https://oeis.org/A021639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/635) mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the nth decimal digit of 1/635, computed by long division. */
seq_t a(idx_t n)
{
    seq_t remainder = 1;        /* current remainder, always < 635 */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        remainder *= 10;        /* bring down next digit */
        digit = remainder / 635;
        remainder = remainder % 635;
    }
    return digit;
}
