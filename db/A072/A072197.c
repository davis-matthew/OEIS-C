/*
 * A072197  a(n) = 4*a(n-1) + 1 with a(0) = 3.
 * https://oeis.org/A072197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 1, a(0) = 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 3;          /* a(0) */
    if (n == 0) return val;
    for (idx_t i = 1; i <= n; ++i)
        val = val * 4 + 1;  /* recurrence step */
    return val;
}
