/*
 * A213480  Number of (w,x,y) with all terms in {0,...,n} and |w-x| + |x-y| !=
 *          w+x+y.
 * https://oeis.org/A213480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^3 + 15*n^2 + 4*n + 5 - (2*n + 5)*((n+1) mod 2)) / 8
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;               /* cast index to seq_t for arithmetic */
    seq_t term = 8 * N * N * N          /* 8*n^3 */
               + 15 * N * N              /* 15*n^2 */
               + 4 * N                   /* 4*n */
               + 5;                     /* constant 5 */
    seq_t parity = (N + 1) & 1;        /* (n+1) mod 2, using bitwise AND */
    seq_t sub = (2 * N + 5) * parity;  /* (2*n + 5)*parity */
    seq_t numerator = term - sub;      /* numerator of the formula */
    return numerator / 8;              /* divide by 8 */
}
