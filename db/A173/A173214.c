/*
 * A173214  Number of ways to place 4 nonattacking amazons (superqueens) on an
 *          n X n board.
 * https://oeis.org/A173214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) =
 *            n^8/24-5n^7/6+47n^6/9+43n^5/10-5053n^4/24+112585n^3/108-15433n^2/8+55669n/270+119917/54
 *            + (n^3/4-21n^2/8+7n-3/2)*(-1)^n + 32/27*(n-1)*cos(2*Pi*n/3) +
 *            40*sqrt(3)*sin(2*Pi*n/3)/81
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 26, u64 n <= 383, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* small values */
    if (n <= 5)
    {
        switch (n)
        {
        case 1: return 0;
        case 2: return 0;
        case 3: return 0;
        case 4: return 0;
        case 5: return 2;
        default: break;
        }
    }
    /* use the explicit polynomial+periodic formula for n>=6 */
    __int128 nn = n;
    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;
    __int128 n4 = n3 * nn;
    __int128 n5 = n4 * nn;
    __int128 n6 = n5 * nn;
    __int128 n7 = n6 * nn;
    __int128 n8 = n7 * nn;

    /* polynomial part scaled by 1080 */
    __int128 poly = (__int128)45 * n8
                  - (__int128)900 * n7
                  + (__int128)5640 * n6
                  + (__int128)4644 * n5
                  - (__int128)227385 * n4
                  + (__int128)1125850 * n3
                  - (__int128)2083455 * n2
                  + (__int128)222676 * nn
                  + (__int128)2398340;

    /* alternating part scaled by 1080 */
    __int128 alt = ((__int128)270 * n3
                   - (__int128)2835 * n2
                   + (__int128)7560 * nn
                   - (__int128)1620);
    if (n & 1) /* odd */
        alt = -alt;

    /* cosine part scaled by 1080 */
    int mod3 = n % 3;
    __int128 cos_part;
    if (mod3 == 0)
        cos_part = (__int128)1280 * (nn - 1);
    else
        cos_part = -(__int128)640 * (nn - 1);

    /* sine part scaled by 1080 */
    __int128 sin_part;
    if (mod3 == 1)
        sin_part = (__int128)800;
    else if (mod3 == 2)
        sin_part = -(__int128)800;
    else
        sin_part = 0;

    __int128 total = poly + alt + cos_part + sin_part;

    return (seq_t)(total / 1080);
}
