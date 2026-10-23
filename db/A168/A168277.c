/*
 * A168277  a(n) = 2*n - (-1)^n - 2.
 * https://oeis.org/A168277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - (-1)^n - 2
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 1, a(3) = 5, ... */

seq_t a(idx_t n)
{
    /* n is 1-indexed; compute 2*n first */
    seq_t two_n = (seq_t)n << 1;   /* equivalent to (seq_t)n * 2 */

    /* If n is odd, (-1)^n = -1  →  a = 2*n - (-1) - 2 = 2*n - 1
       If n is even, (-1)^n =  1  →  a = 2*n -  1 - 2 = 2*n - 3 */
    return (n & 1) ? (two_n - 1) : (two_n - 3);
}
