/*
 * A228889  a(n) = 3*n*(3*n + 1)*(3*n + 2).
 * https://oeis.org/A228889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(3*n + 1)*(3*n + 2)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 541, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)3 * n;  /* t = 3*n */
    return t * (t + 1) * (t + 2);
}
