/*
 * A135923  a(n) = (n^8 - 60*n^6 + 90*n^5 + 1160*n^4 - 3204*n^3 - 5349*n^2 +
 *          26586*n - 23760)/24.
 * https://oeis.org/A135923
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 - 60*n^6 + 90*n^5 + 1160*n^4 - 3204*n^3 - 5349*n^2 +
 *            26586*n - 23760)/24
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 24, u64 n <= 380, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;
    unsigned __int128 n5 = n4 * N;
    unsigned __int128 n6 = n5 * N;
    unsigned __int128 n8 = n6 * n2;

    unsigned __int128 num = n8;
    num -= 60 * n6;
    num += 90 * n5;
    num += 1160 * n4;
    num -= 3204 * n3;
    num -= 5349 * n2;
    num += 26586 * N;
    num -= 23760;
    num /= 24;

    return (seq_t)num;
}
