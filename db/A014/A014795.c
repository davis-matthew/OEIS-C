/*
 * A014795  Squares of odd tetrahedral numbers.
 * https://oeis.org/A014795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((4*n+1)*(4*n+2)*(4*n+3)/6)^2
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 17, u64 n <= 737, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t a1 = 4 * nn + 1;
    seq_t a2 = 4 * nn + 2;
    seq_t a3 = 4 * nn + 3;
    seq_t t  = (a1 * a2 * a3) / 6;
    return t * t;
}
