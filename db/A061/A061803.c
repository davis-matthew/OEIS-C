/*
 * A061803  Sum of n-th row of triangle of 4th powers: 1; 1 16 1; 1 16 81 16
 *          1; 1 16 81 256 81 16 1; ... (cf. A133824).
 * https://oeis.org/A061803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(6*n^4 + 10*n^2 - 1)/15
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 101, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 return 0 (but offset is 1 so not used) */
    if (n < 1)
        return (seq_t)0;
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 N2 = N * N;       /* n^2 */
    unsigned __int128 N4 = N2 * N2;     /* n^4 */
    unsigned __int128 temp = 6 * N4 + 10 * N2 - 1;
    unsigned __int128 res128 = N * temp / 15;
    return (seq_t)res128;
}
