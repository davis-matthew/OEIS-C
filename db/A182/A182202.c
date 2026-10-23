/*
 * A182202  Fibonacci-type sequence based on bitwise inclusive-or: a(0) = 0,
 *          a(1) = 1 and a(n) = a(n-1) + (a(n-1) or a(n-2)).
 * https://oeis.org/A182202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, a(1) = 1, a(n) = a(n-1) + (a(n-1) | a(n-2)) for n >= 2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return 0;
    if (n == 1u) return 1;
    seq_t prev2 = 0;        /* a(n-2) */
    seq_t prev1 = 1;        /* a(n-1) */
    seq_t cur;             /* a(n) */
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 + (prev1 | prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
