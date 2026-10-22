/*
 * A004016  Theta series of planar hexagonal lattice A_2.
 * https://oeis.org/A004016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0) = 1; a(n) = 6 * Sum_{d|n} Kronecker(-3, d) for n > 0, i.e.
 *            6 * (number of divisors d == 1 mod 3 minus number of divisors d
 *            == 2 mod 3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static long long chi3(uint64_t d)
{
    uint64_t r = d % 3;
    if (r == 1) return 1;
    if (r == 2) return -1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    long long s = 0;
    uint64_t d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            uint64_t e = n / d;
            s += chi3(d);
            if (e != d) s += chi3(e);
        }
    }
    seq_t r = (seq_t)(unsigned long long)s;
    return r * 6;
}
