/*
 * A022369  Fibonacci sequence beginning 2, 14.
 * https://oeis.org/A022369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 2, a(1) = 14
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the terms of OEIS A022369 using a simple linear recurrence. */
seq_t
a (idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)14;

    /* Two previous terms */
    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)14;
    seq_t cur   = 0;

    /* Iterate up to n; idx_t is unsigned, so this loop is safe. */
    for (idx_t i = 2; i <= n; ++i)
    {
        cur   = prev2 + prev1;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
