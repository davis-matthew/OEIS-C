/*
 * A141582  a(n) = number of ways to dispose two pawns on a chessboard of size
 *          n X n (two dispositions are equivalent if one can be rotated or
 *          reflected to give the other one).
 * https://oeis.org/A141582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   For n even, a(n) = n*(n^3 + 6*n - 4)/16; For n odd, a(n) = (n^2
 *            - 1)*(n^2 + 7)/16; Returns zero for n=0,1 as the formulas
 *            naturally give
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 271, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if ((n & 1) == 0) {                 /* n is even */
        seq_t n3 = sn * sn * sn;         /* n^3 */
        seq_t inner = n3 + sn * 6 - (seq_t)4; /* n^3 + 6*n - 4 */
        return sn * inner / 16;         /* n * (n^3 + 6*n - 4) / 16 */
    } else {                            /* n is odd */
        seq_t n2 = sn * sn;             /* n^2 */
        return (n2 - 1) * (n2 + 7) / 16;/* (n^2-1)*(n^2+7) / 16 */
    }
}
