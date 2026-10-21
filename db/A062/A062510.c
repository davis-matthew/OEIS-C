/*
 * A062510  a(n) = 2^n + (-1)^(n+1).
 * https://oeis.org/A062510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + (-1)^(n+1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 <<= 1;          /* multiply by 2 each iteration */
    }
    if ((n & 1) == 0) {
        /* n even: (-1)^(n+1) = -1 */
        return pow2 - 1;
    } else {
        /* n odd: (-1)^(n+1) = +1 */
        return pow2 + 1;
    }
}
