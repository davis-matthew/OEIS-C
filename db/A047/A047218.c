/*
 * A047218  Numbers that are congruent to {0, 3} mod 5.
 * https://oeis.org/A047218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + (-1)^n - 9)/4
 * Range:     u8 n <= 26, u16 n <= 6557, u32 n <= 7224, u64 n <= 7224,
 *            u128 n <= 7224, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 10*n - (9 - (-1)^n) and divide by 4. */
    seq_t t = (seq_t)10 * n;
    /* For even n subtract 8, for odd n subtract 10. */
    if ((n & 1) == 0)
        t -= (seq_t)8;
    else
        t -= (seq_t)10;
    return t / (seq_t)4;
}
