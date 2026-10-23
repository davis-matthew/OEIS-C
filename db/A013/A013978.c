/*
 * A013978  Molien series of 4-dimensional representation of u.g.g.r. #8.
 * https://oeis.org/A013978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^3+12*n^2+27*n+18)/18 - (n mod 2)*(2*n+1)/2 -
 *            ((2*n^2+2*n) mod 3)*(12*n+7)/9 - ((2*n^2+n) mod 3)*2/9
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 905, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t t1 = (8*N*N*N + 12*N*N + 27*N + 18) / 18;
    seq_t t2 = ((N & 1) * (2*N + 1)) / 2;
    seq_t mod3_1 = ((2*N*N + 2*N) % 3);
    seq_t t3 = (mod3_1 * (12*N + 7)) / 9;
    seq_t mod3_2 = ((2*N*N + N) % 3);
    seq_t t4 = (mod3_2 * 2) / 9;
    return t1 - t2 - t3 - t4;
}
