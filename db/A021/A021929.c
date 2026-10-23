/*
 * A021929  Decimal expansion of 1/925.
 * https://oeis.org/A021929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/925) % 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    uint64_t rem = 1;   /* remainder before the current digit */
    seq_t digit = 0;    /* digit at the last step */

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;
        digit = rem / 925;
        rem %= 925;
    }
    return digit;
}
