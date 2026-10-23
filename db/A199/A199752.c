/*
 * A199752  a(n) = 3*11^n+1.
 * https://oeis.org/A199752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*11^n + 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 4, a(n) = 11*a(n-1) - 10 */

seq_t a(idx_t n)
{
    seq_t result = 1;         /* 11^0 */
    for (idx_t i = 0; i < n; ++i)
        result *= 11;          /* compute 11^n */

    return result * 3 + 1;     /* 3*11^n + 1 */
}
