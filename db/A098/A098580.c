/*
 * A098580  Expansion of (sqrt(1-8*x)-4*x)/sqrt(1-8*x).
 * https://oeis.org/A098580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 22, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098580:
   a(0) = 1
   a(1) = -4
   For n ≥ 2: a(n) = 4*(2*n-3)/(n-1) * a(n-1)
   (equivalently: a(n) = -2^(n+1) * (2*(n-1))! / ((n-1)!^2) )
*/

#include <stdint.h>

/* Euclidean gcd for signed 64‑bit integers */
static int64_t gcd64(int64_t a, int64_t b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-4;

    seq_t res = (seq_t)-4;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        /* multiplier = 4*(2*i-3)/(i-1) */
        int64_t num = 4LL * (2LL * (long long)i - 3LL);   /* positive */
        int64_t den = (long long)i - 1LL;

        /* cancel common factors between num and den */
        int64_t g = gcd64(num, den);
        num /= g;
        den /= g;

        /* cancel any remaining factor of den from res using modulo */
        if (den != 1) {
            seq_t rem = res % (seq_t)den;
            int64_t g2 = gcd64((long long)(rem >= 0 ? rem : -rem), den);
            if (g2 != 1) {
                res /= (seq_t)g2;
                den /= g2;
            }
        }

        /* multiply by the reduced numerator and divide by the reduced denominator */
        res = (res * (seq_t)num) / (seq_t)den;
    }

    return res;
}
