/*
 * A153774  a(2*n) = 3*a(2*n-1), a(2*n+1) = 3*a(2*n) - 1, with a(1) = 1.
 * https://oeis.org/A153774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n>1, a(n) = 3*a(n-1) if n is even a(n) = 3*a(n-1)
 *            - 1 if n is odd
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t val = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
    {
        if ((i & (idx_t)1) == 0)
            val = val * (seq_t)3;
        else
            val = val * (seq_t)3 - (seq_t)1;
    }
    return val;
}
