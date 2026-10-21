/*
 * A063914  Odd numbers interlaced with numbers 3m+2.
 * https://oeis.org/A063914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n - 2 + (-1)^n*(n - 2))/4
 * Range:     u8 n <= 119, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For safety we return 0 for n == 0. */
    if (n == 0) return 0;

    /* If n is odd, a(n) = n. */
    if (n & 1)
        return (seq_t)n;

    /* If n is even, a(n) = (3*n - 2) / 2. */
    seq_t nn = (seq_t)n;        /* Convert index to seq_t. */
    seq_t three_n = nn * 3;     /* 3 * n. */
    three_n -= (seq_t)2;        /* Subtract 2. */
    return three_n >> 1;        /* Divide by 2 (integer division). */
}
