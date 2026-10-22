/*
 * A000033  a(n) = A058087(n,n-2).
 * https://oeis.org/A000033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   recurrence (n-3)*(n-2)*(2*n-5)*(2*n-7)*a(n) =
 *            (n-3)*(n-2)*n*(2*n-7)^2*a(n-1) + (n-4)*(n-3)*n*(2*n-3)^2*a(n-2)
 *            + (n-2)*n*(2*n-5)*(2*n-3)*a(n-3), a(1) = 0, a(2) = 2, a(3) = 3,
 *            a(4) = 4, a(5) = 40, a(6) = 210. The division is split into
 *            quotient/remainder parts so that no intermediate value is
 *            much...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t a1, a2, a3;
    idx_t m;

    if (n <= 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 3;
    if (n == 4) return 4;
    if (n == 5) return 40;
    if (n == 6) return 210;

    a3 = 4;
    a2 = 40;
    a1 = 210;
    for (m = 7; m <= n; m++) {
        seq_t res;
        if (m <= 150) {
            unsigned long long M = (unsigned long long)m;
            unsigned long long d = (M - 3) * (M - 2) * (2 * M - 5) * (2 * M - 7);
            unsigned long long c[3];
            seq_t av[3];
            seq_t ds = (seq_t)d;
            unsigned long long rem = 0;
            int i;

            if ((unsigned long long)ds != d) return 0; /* value cannot fit seq_t */

            c[0] = (M - 3) * (M - 2) * M * (2 * M - 7) * (2 * M - 7);
            c[1] = (M - 4) * (M - 3) * M * (2 * M - 3) * (2 * M - 3);
            c[2] = (M - 2) * M * (2 * M - 5) * (2 * M - 3);
            av[0] = a1;
            av[1] = a2;
            av[2] = a3;

            res = 0;
            for (i = 0; i < 3; i++) {
                unsigned long long u = c[i] / d, v = c[i] % d;
                seq_t q = av[i] / ds;
                unsigned long long r = (unsigned long long)(av[i] % ds);
                res += (seq_t)u * av[i] + (seq_t)v * q;
                rem += v * r;
            }
            res += (seq_t)(rem / d);
        } else {
            seq_t M = (seq_t)m;
            seq_t d = (M - 3) * (M - 2) * (2 * M - 5) * (2 * M - 7);
            seq_t c1 = (M - 3) * (M - 2) * M * (2 * M - 7) * (2 * M - 7);
            seq_t c2 = (M - 4) * (M - 3) * M * (2 * M - 3) * (2 * M - 3);
            seq_t c3 = (M - 2) * M * (2 * M - 5) * (2 * M - 3);
            if (d == 0) return 0;
            res = (c1 * a1 + c2 * a2 + c3 * a3) / d;
        }
        a3 = a2;
        a2 = a1;
        a1 = res;
    }
    return a1;
}
