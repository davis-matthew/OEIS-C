/*
 * A140986  Number of n-colorings of the cubical graph.
 * https://oeis.org/A140986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^8-12*n^7+66*n^6-214*n^5+441*n^4-572*n^3+423*n^2-133*n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 17, u64 n <= 257, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    __int128 nn = (__int128)n;
    __int128 res = 1;
    res = res * nn + (__int128)-12;
    res = res * nn + (__int128)66;
    res = res * nn + (__int128)-214;
    res = res * nn + (__int128)441;
    res = res * nn + (__int128)-572;
    res = res * nn + (__int128)423;
    res = res * nn + (__int128)-133;
    res = res * nn;          /* final multiplication by n (coeff 0) */
    return (seq_t)res;
}
