/*
 * A047317  Numbers that are congruent to {1, 2, 4, 5, 6} mod 7.
 * https://oeis.org/A047317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7*n-1)/5)
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 7;   /* 7 * n */
    t -= 1;                  /* 7*n - 1 */
    t /= 5;                  /* floor division by 5 */
    return t;
}
