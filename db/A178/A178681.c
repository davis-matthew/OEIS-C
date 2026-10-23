/*
 * A178681  a(n) = 6^n + 6.
 * https://oeis.org/A178681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n + 6 Recurrence used: a(0) = 7, a(n) = 6 * (a(n-1) -
 *            5) for n > 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)7;           /* a(0) */
    if (n == 0) {
        return res;
    }
    /* iterative application of the recurrence */
    for (idx_t i = 1; i <= n; ++i) {
        res = (seq_t)6 * (res - (seq_t)5);
    }
    return res;
}
