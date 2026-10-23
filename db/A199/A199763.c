/*
 * A199763  a(n) = 9*11^n + 1.
 * https://oeis.org/A199763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 9*11^n + 1
 * Recurrence: a(0) = 10, a(n) = 11*a(n-1) - 10
 * (Equivalent to the closed form above.)
 */
seq_t a(idx_t n)
{
    seq_t res = 10;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
        res = 11 * res - 10; /* recurrence step */
    return res;
}
