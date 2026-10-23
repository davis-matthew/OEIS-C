/*
 * A168456  a(n) = (10*n - 5*(-1)^n + 1)/2.
 * https://oeis.org/A168456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 + 10*floor((n-1)/2) (for n ≥ 1)
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (n-1)/2 is the integer division */
    seq_t half = (seq_t)((n - 1) / 2);
    return (seq_t)8 + (seq_t)10 * half;
}
