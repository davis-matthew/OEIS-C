/*
 * A228743  Number of arrays of the median of three adjacent elements of some
 *          length 8 0..n array.
 * https://oeis.org/A228743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(11*n^5+199*n^4+706*n^3+824*n^2+420*n+90)/90
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 54, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 N = (unsigned __int128)n;                  /* promoted index */
    unsigned __int128 num = (unsigned __int128)11 * N * N * N * N * N
                         + (unsigned __int128)199 * N * N * N * N
                         + (unsigned __int128)706 * N * N * N
                         + (unsigned __int128)824 * N * N
                         + (unsigned __int128)420 * N
                         + (unsigned __int128)90;              /* 11*n^5+...+90   */
    unsigned __int128 prod = (N + 1) * num;                       /* multiply (n+1)  */
    prod /= 90;                                                   /* then divide by 90 */
    return (seq_t)prod;                                           /* cast to seq_t  */
}
