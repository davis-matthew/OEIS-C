/*
 * A144945  Number of ways to place 2 queens on an n X n chessboard so that
 *          they attack each other.
 * https://oeis.org/A144945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)*n*(5*n-1)/3
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 1370, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sn = (seq_t)n;
    seq_t n_minus1 = sn - 1;
    seq_t num = n_minus1 * sn * (((seq_t)5 * sn) - 1);
    return num / 3;
}
