/*
 * A047578  Numbers that are congruent to {2, 5, 6, 7} mod 8.
 * https://oeis.org/A047578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - cos(pi*n/2) (cos cycle 1,0,-1,0)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t two_n = (seq_t)n * (seq_t)2;
    switch (n & 3) {
        case 0:  return two_n - 1;
        case 2:  return two_n + 1;
        default: return two_n;
    }
}
