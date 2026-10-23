/*
 * A179425  Number of ways to place 5 nonattacking kings on an n X n toroidal
 *          board.
 * https://oeis.org/A179425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^8 - 90*n^6 + 3155*n^4 - 51450*n^2 + 332544)/120 *
 *            valid for n >= 7; a(5) = 10; a(n) = 0 for n < 5
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 15, u64 n <= 136, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* handle indices below the first non‑zero term */
    if (n < 5) return (seq_t)0;
    if (n == 5) return (seq_t)10;

    /* all arithmetic in seq_t to avoid overflow of smaller integer types */
    seq_t ns  = (seq_t)n;
    seq_t n2  = ns * ns;         /* n² */
    seq_t n4  = n2 * n2;         /* n⁴ */
    seq_t n6  = n4 * n2;         /* n⁶ */
    seq_t n8  = n4 * n4;         /* n⁸ */

    /* P = n⁸ – 90·n⁶ + 3155·n⁴ – 51450·n² + 332544 */
    seq_t P   = n8;
    P -= (seq_t)90 * n6;
    P += (seq_t)3155 * n4;
    P -= (seq_t)51450 * n2;
    P += (seq_t)332544;

    /* cancel the factor 120 (2³·3·5) with n² and P to keep intermediates small */
    seq_t d1 = (seq_t)120;
    seq_t g  = gcd(n2, d1);
    d1 /= g;
    n2 /= g;

    g = gcd(P, d1);
    d1 /= g;
    P  /= g;

    /* now d1 should be 1; if not, divide once more */
    seq_t result = n2 * P;
    if (d1 != (seq_t)1)
        result /= d1;

    return result;
}
