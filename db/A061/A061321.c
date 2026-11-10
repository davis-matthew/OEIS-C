/*
 * A061321  Column 5 of A061315.
 * https://oeis.org/A061321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A061320(n)/5 =
 *            n*(n+1)*(n^8+4*n^7+14*n^6+28*n^5+77*n^4+112*n^3+196*n^2+144*n+2304)*(n^2+n+4)*(n^4+2*n^3+5*n^2+4*n+36)/1658880
 * Range:     u8 none, u16 n <= 0, u32 n <= 9, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A061321_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

/* multiply acc by f after cancelling common factors with the pending divisor d */
static seq_t A061321_mulred(seq_t acc, seq_t f, seq_t *d)
{
    seq_t g = A061321_gcd(f, *d);
    if (g != 0) {
        f /= g;
        *d /= g;
    }
    return acc * f;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;
    seq_t n7 = n6 * nn;
    seq_t n8 = n7 * nn;

    seq_t p1 = nn;
    seq_t p2 = nn + 1;
    seq_t p3 = n8 + 4 * n7 + 14 * n6 + 28 * n5 + 77 * n4 + 112 * n3
               + 196 * n2 + 144 * nn + 2304;
    seq_t p4 = n2 + nn + 4;
    seq_t p5 = n4 + 2 * n3 + 5 * n2 + 4 * nn + 36;

    seq_t d = 1658880; /* 331776 * 5 */
    seq_t r = 1;
    r = A061321_mulred(r, p1, &d);
    r = A061321_mulred(r, p2, &d);
    r = A061321_mulred(r, p4, &d);
    r = A061321_mulred(r, p5, &d);
    r = A061321_mulred(r, p3, &d);
    return r / d;
}
