/*
 * A214229  a(n) equals gcd(r,2*n+1) where r is 1 + (A143608(i+1) mod (2*n+1))
 *          where A143608(i) is the first zero mod 2n+1 other than 0.
 * https://oeis.org/A214229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with m=2n+1, i = least i>0 with A143608(i) == 0 (mod m); a(n) =
 *            gcd(1 + A143608(i+1) mod m, m). A143608:
 *            a(0)=0,a(1)=1,a(2)=4,a(3)=7, a(k)=6*a(k-2)-a(k-4). Computed by
 *            iterating the recurrence mod m
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A214229_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)2 * (seq_t)n + 1;
    seq_t p0 = 0;
    seq_t p1 = (seq_t)1 % m;
    seq_t p2 = (seq_t)4 % m;
    seq_t p3 = (seq_t)7 % m;
    for (;;) {
        /* advance window: (a(i..i+3)) -> (a(i+1..i+4)) */
        seq_t nw = (((seq_t)6 * p2) % m + m - p0) % m;
        p0 = p1;
        p1 = p2;
        p2 = p3;
        p3 = nw;
        if (p0 == 0) {
            seq_t r = 1 + p1;
            return A214229_gcd(r, m);
        }
    }
}
