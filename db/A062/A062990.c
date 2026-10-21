/*
 * A062990  Eighth column (r=7) of FS(5) staircase array A062985.
 * https://oeis.org/A062990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n^4 + 29*n^3 + 326*n^2 + 1744*n +
 *            4200)/5040
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate values, then cast to seq_t. */
    __int128 nn  = (__int128)n;           /* n */
    __int128 n2  = nn * nn;                /* n² */
    __int128 n3  = n2 * nn;                /* n³ */
    __int128 n4  = n3 * nn;                /* n⁴ */

    /* Polynomial part: n⁴ + 29*n³ + 326*n² + 1744*n + 4200 */
    __int128 poly = n4
                    + (__int128)29 * n3
                    + (__int128)326 * n2
                    + (__int128)1744 * nn
                    + 4200;

    /* Factor (n+1)(n+2)(n+3) */
    __int128 t1 = nn + 1;
    __int128 t2 = nn + 2;
    __int128 t3 = nn + 3;
    __int128 num = t1 * t2 * t3;

    /* Full numerator */
    __int128 value = num * poly;

    /* Divide by 7! = 5040; the division is exact. */
    __int128 result = value / 5040;

    return (seq_t)result;
}
