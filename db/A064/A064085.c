/*
 * A064085  Quotient of A000225 and A064084.
 * https://oeis.org/A064085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000225(n) / A064084(n) = (2^n - 1) / Product_{p^e || n}
 *            (2^(p^e) - 1). Computed exactly as the polynomial quotient Q(x)
 *            = (x^n - 1) / ((x - 1) * Product_{p^e || n} (x^(p^e) - 1)/(x -
 *            1)), which is an integer polynomial (the product of Phi_d(x)
 *            over the divisors d of n that are not prime p...
 * Range:     u8 n <= 19, u16 n <= 27, u32 n <= 51, u64 n <= 83, u128 n <= 153,
 *            big any n
 * Validated: standard, pass, 63 terms through n = 986 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    uint64_t N = (uint64_t)n;
    long long *c = (long long *)malloc((size_t)(N + 1) * sizeof(long long));
    if (!c)
        return 0;
    uint64_t i;
    for (i = 0; i <= N; i++)
        c[i] = 0;
    c[N] = 1;
    c[0] = -1;
    uint64_t deg = N;

    /* divide by (x - 1): synthetic division */
    for (i = deg; i >= 1; i--)
        c[i - 1] += c[i];
    for (i = 0; i < deg; i++)
        c[i] = c[i + 1];
    deg -= 1;

    /* factor n and divide by S_q(x) = 1 + x + ... + x^(q-1) for each prime power q || n */
    uint64_t m = N;
    uint64_t d;
    for (d = 2; d <= m; d++) {
        if (d * d > m)
            d = m; /* remaining m is prime */
        if (m % d != 0)
            continue;
        uint64_t q = 1;
        while (m % d == 0) {
            m /= d;
            q *= d;
        }
        /* long division by monic all-ones polynomial of degree q-1 */
        uint64_t s = q - 1;
        if (s > 0 && s <= deg) {
            uint64_t k;
            for (i = deg; i + 1 > s; i--) {
                long long t = c[i];
                if (t != 0) {
                    for (k = 1; k <= s; k++)
                        c[i - k] -= t;
                }
                if (i == s)
                    break;
            }
            for (i = 0; i + s <= deg; i++)
                c[i] = c[i + s];
            deg -= s;
        }
        if (m == 1)
            break;
    }

    /* evaluate at x = 2, splitting positive and negative parts */
    seq_t P = 0, Ng = 0;
    i = deg + 1;
    while (i-- > 0) {
        long long v = c[i];
        P = P * 2;
        Ng = Ng * 2;
        if (v > 0)
            P += (idx_t)v;
        else if (v < 0)
            Ng += (idx_t)(-v);
    }
    free(c);
    return P - Ng;
}
