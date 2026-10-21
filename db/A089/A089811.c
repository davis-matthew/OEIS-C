/*
 * A089811  Expansion of Jacobi theta function (3*theta_4(q^18) -
 *          theta_4(q^2))/2.
 * https://oeis.org/A089811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 + sum_{m>=1} [ -(-1)^m * δ_{n,2m^2} + 3*(-1)^m *
 *            δ_{n,18m^2} ]
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the expansion of the Jacobi theta function (3*theta_4(q^18)-theta_4(q^2))/2 */

#include <math.h>

static unsigned long long isqrt_uint64(unsigned long long x)
{
    unsigned long long r = (unsigned long long) sqrt((long double) x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;           /* a(0) = 1 */

    seq_t res = 0;

    /* Contribution from 2*m^2 terms */
    if ((n & 1ULL) == 0) {
        unsigned long long half = n >> 1;            /* n/2 */
        unsigned long long k = isqrt_uint64(half);
        if (k * k == half) {
            seq_t s = (k & 1ULL) ? -1 : 1;          /* (-1)^k */
            res -= s;                               /* -(-1)^k */
        }
    }

    /* Contribution from 18*m^2 terms */
    if ((n % 18ULL) == 0) {
        unsigned long long part = n / 18ULL;
        unsigned long long u = isqrt_uint64(part);
        if (u * u == part) {
            seq_t s = (u & 1ULL) ? -1 : 1;          /* (-1)^u */
            res += 3 * s;                           /* 3*(-1)^u */
        }
    }

    return res;
}
