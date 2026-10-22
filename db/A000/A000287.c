/*
 * A000287  Number of rooted polyhedral graphs with n edges.
 * https://oeis.org/A000287
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, nice
 * Formula:   2*(n-1)*a(n) = (3*n-21)*a(n-1) + (16*n-72)*a(n-2) +
 *            (15*n-63)*a(n-3) + (4*n-14)*a(n-4), for n >= 10, with a(6) = 1,
 *            a(7) = 0, a(8) = 4, a(9) = 6. (This is the OEIS recurrence
 *            multiplied by 2. Each a(n-i) is split into quotient and
 *            remainder modulo 2*(n-1). That keeps every intermediate value <=
 *            a...
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 25, u64 n <= 42, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t w = 1, x = 0, y = 4, z = 6; /* a(k-4), a(k-3), a(k-2), a(k-1) */
    idx_t k;

    if (n <= 6) return w;
    if (n == 7) return x;
    if (n == 8) return y;
    if (n == 9) return z;

    for (k = 10; k <= n; k++) {
        unsigned long long D  = 2ULL * (unsigned long long)(k - 1);
        unsigned long long c1 = 3ULL * k - 21ULL;
        unsigned long long c2 = 16ULL * k - 72ULL;
        unsigned long long c3 = 15ULL * k - 63ULL;
        unsigned long long c4 = 4ULL * k - 14ULL;
        seq_t s;
        unsigned long long r;

        s = (z / D) * c1;
        s += (y / D) * c2;
        s += (x / D) * c3;
        s += (w / D) * c4;

        r = (unsigned long long)(z % D) * c1
          + (unsigned long long)(y % D) * c2
          + (unsigned long long)(x % D) * c3
          + (unsigned long long)(w % D) * c4;
        s += r / D;

        w = x;
        x = y;
        y = z;
        z = s;
    }
    return z;
}
