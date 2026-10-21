/*
 * A219501  Number of n X 7 arrays of the minimum value of corresponding
 *          elements and their horizontal or vertical neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..1 n X 7 array.
 * https://oeis.org/A219501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/720*n^6 + 1/240*n^5 + 23/144*n^4 + 13/16*n^3 +
 *            331/180*n^2 + 311/60*n - 27 (valid for n>3)
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 7;
    if (n == 2) return 13;
    if (n == 3) return 45;

    /* compute the numerator of the rational formula using 128-bit integers */
    __int128 N = n;
    __int128 n2 = N * N;
    __int128 n3 = n2 * N;
    __int128 n4 = n3 * N;
    __int128 n5 = n4 * N;
    __int128 n6 = n5 * N;

    __int128 num = 0;
    num += n6;                                  // n^6 / 720
    num += (__int128)3 * n5;                    // 3*n^5 / 720
    num += (__int128)23 * 5 * n4;               // 23*5*n^4 / 720
    num += (__int128)13 * 45 * n3;              // 13*45*n^3 / 720
    num += (__int128)331 * 4 * n2;              // 331*4*n^2 / 720
    num += (__int128)311 * 12 * N;              // 311*12*n / 720
    num += (__int128)(-19440);                  // -27*720 / 720

    /* divide by 720 to get the exact integer result */
    seq_t result = (seq_t)(num / 720);
    return result;
}
