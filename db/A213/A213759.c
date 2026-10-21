/*
 * A213759  Principal diagonal of the convolution array A213783.
 * https://oeis.org/A213759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 - 3*(-1)^n - 4*n + 18*n^2 + 4*n^3)/24, offset 1
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t sn2  = sn * sn;
    seq_t sn3  = sn2 * sn;
    seq_t t1   = (n & 1) ? (seq_t)6 : (seq_t)0;           /* 6 if n odd, else 0 */
    seq_t big  = sn3 * (seq_t)4 + sn2 * (seq_t)18;        /* 4*n^3 + 18*n^2 */
    seq_t sub  = sn * (seq_t)4;                          /* 4*n */
    seq_t sum  = big - sub + t1;                         /* final numerator */
    return sum / (seq_t)24;
}
