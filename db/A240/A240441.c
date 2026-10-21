/*
 * A240441  Number of ways to place 4 points on a triangular grid of side n so
 *          that no three of these points are vertices of an equilateral
 *          triangle of any orientation.
 * https://oeis.org/A240441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 + 4*n^7 - 14*n^6 - 56*n^5 + 61*n^4 + 220*n^3 -
 *            84*n^2 - 240*n)/384 + IF(n odd)*(6*n + 3)/32
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 33, u64 n <= 538, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safeguard for out‑of‑range argument */

    /* use signed 128‑bit integer for intermediate calculations */
    __int128 nn = (__int128)n;

    /* Horner evaluation of the 8th‑degree polynomial */
    static const int coeffs[9] = {1, 4, -14, -56, 61, 220, -84, -240, 0};
    __int128 P = 0;
    for (int i = 0; i < 9; ++i)
        P = P * nn + coeffs[i];

    /* For odd n add the correction term 12*(6*n+3) = 72*n + 36 */
    if ((n & 1) != 0)
        P += 72 * nn + 36;

    /* Divide by 384 – the result is guaranteed to be integer after the
       correction above, even for odd n. */
    return (seq_t)(P / 384);
}
