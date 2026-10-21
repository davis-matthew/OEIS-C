/*
 * A098135  a(h) = d(h,j) = lcm( f(h,j,1) ... f(h,j,h) ), when j=3.
 * https://oeis.org/A098135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{ (4*(n+1)-3*a) / (gcd(a,n+1) * gcd(4, (a/gcd(a,n+1)))
 *            ) | 1 <= a <= n+1 } for j=3 (tabular result: a(0)=1, a(1)=5,
 *            a(2)=9, ...)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"   /* provides static seq_t gcd(seq_t a, seq_t b) */

static uint64_t gcd_u64(uint64_t x, uint64_t y)
{
    while (y) {
        uint64_t t = x % y;
        x   = y;
        y   = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* h = n+1 corresponds to the h in the definition */
    idx_t h = n + 1;

    seq_t lcm_val = (seq_t)1;

    for (idx_t a_val = 1; a_val <= h; ++a_val) {
        /* Compute g = gcd(a, h) and d = gcd(4, a/g) */
        uint64_t a_u = a_val;
        uint64_t h_u = h;

        uint64_t g = gcd_u64(a_u, h_u);
        uint64_t t = a_u / g;
        uint64_t d = gcd_u64(4, t);

        uint64_t denom = g * d;                 /* denominator of the term */
        uint64_t numer = 4 * h_u - 3 * a_u;     /* numerator of the term */

        uint64_t term_u = numer / denom;        /* term fits in 64 bits */
        seq_t term = (seq_t)term_u;

        /* Update running lcm: lcm(lcm_val, term) = lcm_val / gcd * term */
        seq_t g2 = gcd(lcm_val, term);           /* gcd for seq_t */
        lcm_val = (lcm_val / g2) * term;
    }

    return lcm_val;
}
