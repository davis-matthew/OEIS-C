/*
 * A047270  Numbers that are congruent to {3, 5} mod 6.
 * https://oeis.org/A047270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n - (-1)^n - 1) / 2
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 3; a(2) = 5; a(n) = 3*n     when n is odd,
       a(n) = 3*n - 1 when n is even          */
    seq_t val = (seq_t)n * (seq_t)3;
    if ((n & 1) == 0)    /* even n */
        val -= (seq_t)1;
    return val;
}
