/*
 * A199027  a(n) = 8*11^n-1.
 * https://oeis.org/A199027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*11^n-1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 7; a(n) = 11*a(n-1) + 10 for n > 0 */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)7;
    idx_t i;
    for (i = 1; i <= n; ++i)
        val = (seq_t)11 * val + (seq_t)10;
    return val;
}
