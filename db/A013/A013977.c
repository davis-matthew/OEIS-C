/*
 * A013977  Molien series of 4-dimensional representation of u.g.g.r. #9.
 * https://oeis.org/A013977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (32*n^3+24*n^2+27*n+9)/9 - ((2*n^2+n) mod 3)*(8*n+1)/3 -
 *            (n mod 3)*2/9
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 540, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* Common numerator: (32*n^3 + 24*n^2 + 27*n + 9)
       - 3 * ((2*n^2 + n) mod 3) * (8*n + 1)
       - (n mod 3) * 2       */
    seq_t part1  = (seq_t)32 * N * N * N
                 + (seq_t)24 * N * N
                 + (seq_t)27 * N
                 + 9;

    seq_t r1 = ((seq_t)2 * N * N + N) % 3;          /* ((2*n^2 + n) mod 3) */
    seq_t term2 = 3 * r1 * ((seq_t)8 * N + 1);      /* 3 * r1 * (8*n + 1)  */

    seq_t r2 = (N % 3) * 2;                       /* (n mod 3) * 2         */

    seq_t numerator = part1 - term2 - r2;
    return numerator / 9;
}
