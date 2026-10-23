/*
 * A016790  a(n) = (3n+2)^2.
 * https://oeis.org/A016790
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 2)^2, offset -1 (first term at n = -1)
 * Range:     u8 n <= 4, u16 n <= 84, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The only negative index that may appear is -1; its value is 1. */
    if (n == -1)
        return (seq_t)1;
    seq_t t = (seq_t)(3 * n + 2);
    return t * t;
}
