/*
 * A014641  Odd octagonal numbers: (2n+1)*(6n+1).
 * https://oeis.org/A014641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(6*n+1)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (nn*2 + 1) * (nn*6 + 1);
}
