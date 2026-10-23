/*
 * A047235  Numbers that are congruent to {2, 4} mod 6.
 * https://oeis.org/A047235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - (n%2==0 ? 2 : 1) (index starts at 1)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t to avoid overflow of intermediate products */
    seq_t val = (seq_t)3 * (seq_t)n;
    if ((n & (idx_t)1) == 0)  /* even n */
        val -= (seq_t)2;
    else                      /* odd n */
        val -= (seq_t)1;
    return val;
}
