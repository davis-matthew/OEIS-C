/*
 * A078548  a(n) = lcm(n, A052429(n)) - A052429(n).
 * https://oeis.org/A078548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = lcm(n, A052429(n)) - A052429(n), where A052429(n) = lcm
 *            of the nonzero decimal digits of n
 * Range:     u8 n <= 26, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A078548_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static seq_t A052429_a(idx_t n)
{
    seq_t lcm = 1;
    idx_t x = n;
    while (x > 0) {
        int d = (int)(x % 10);
        x /= 10;
        if (d == 0) continue;
        seq_t g = A078548_gcd(lcm, (seq_t)d);
        lcm = lcm / g * (seq_t)d;
    }
    return lcm;
}

seq_t a(idx_t n)
{
    seq_t L = A052429_a(n);
    seq_t N = (seq_t)n;
    seq_t g = A078548_gcd(N, L);
    seq_t l = N / g * L;
    return l - L;
}
