/*
 * A199110  a(n) = 7*3^n + 1.
 * https://oeis.org/A199110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * 3^n + 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used in code: a(0) = 8, a(n) = 3 * a(n-1) - 2 for n >= 1 */

seq_t a(idx_t n)
{
    seq_t val = 8;            /* a(0) */
    if (n == 0)
        return val;
    for (idx_t i = 1; i <= n; ++i)
        val = 3 * val - 2;    /* a(i) = 3*a(i-1) - 2 */
    return val;
}
