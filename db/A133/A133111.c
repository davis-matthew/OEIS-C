/*
 * A133111  a(n) = (1/(1!*2!*3!*4!))*Sum_{1 <= x_1, x_2, x_3, x_4 <= n} |det
 *          V(x_1,x_2,x_3,x_4)|, where V(x_1,x_2,x_3,x_4) is the Vandermonde
 *          matrix of order 4.
 * https://oeis.org/A133111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2*(n^2-1)^2*(n^2-4)*(n^2-9)/302400
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 32, u64 n <= 298, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t num = (seq_t)1;
    seq_t d   = (seq_t)302400;

    /* factor n^2 */
    {
        seq_t f = n2;
        seq_t g = gcd(f, d);
        f /= g;
        d /= g;
        num = num * f;
    }

    /* factor (n^2-1) first occurrence */
    {
        seq_t f = n2 - (seq_t)1;
        seq_t g = gcd(f, d);
        f /= g;
        d /= g;
        num = num * f;
    }

    /* factor (n^2-1) second occurrence */
    {
        seq_t f = n2 - (seq_t)1;
        seq_t g = gcd(f, d);
        f /= g;
        d /= g;
        num = num * f;
    }

    /* factor (n^2-4) */
    {
        seq_t f = n2 - (seq_t)4;
        seq_t g = gcd(f, d);
        f /= g;
        d /= g;
        num = num * f;
    }

    /* factor (n^2-9) */
    {
        seq_t f = n2 - (seq_t)9;
        seq_t g = gcd(f, d);
        f /= g;
        d /= g;
        num = num * f;
    }

    /* the denominator must now be 1; if not, divide */
    if (d != (seq_t)1)
    {
        num = num / d;
    }

    return num;
}
