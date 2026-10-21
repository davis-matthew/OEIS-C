/*
 * A224444  Multiplicities for representations of nonnegative numbers as
 *          primitive sums of three squares of integers (square 0 allowed).
 * https://oeis.org/A224444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primitive representations of n as a^2 + b^2 +
 *            c^2 * with 0 ≤ a ≤ b ≤ c, gcd(a,b,c) = 1. * * Algorithm: * For
 *            every a (0 ≤ a ≤ √n) compute rest = n - a². * For every b (a ≤ b
 *            ≤ √rest) compute rem = rest - b². * Let c = ⌊√rem⌋. If c < b or
 *            c² ≠ rem, the triple does not satisfy the...
 * Range:     u8 n <= 125, u16 n <= 125, u32 n <= 125, u64 n <= 125,
 *            u128 n <= 125, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Integer square root for unsigned 64‑bit values (floor). */
static inline idx_t isqrt_u64(idx_t x)
{
    if (x == 0) return 0;
    idx_t r = x;
    idx_t t = (r + 1) >> 1;
    while (t < r) {
        r = t;
        t = (r + x / r) >> 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* 0 = 0^2 + 0^2 + 0^2 is not primitive */

    seq_t count = 0;

    idx_t amax = isqrt_u64(n);
    for (idx_t a = 0; a <= amax; ++a) {
        idx_t rest = n - a * a;          /* n - a^2, non‑negative */
        idx_t bmax = isqrt_u64(rest);
        for (idx_t b = a; b <= bmax; ++b) {
            idx_t rem = rest - b * b;    /* remaining part, non‑negative */
            idx_t c = isqrt_u64(rem);
            if (c < b) continue;        /* enforce a ≤ b ≤ c */
            if (c * c != rem) continue; /* c must satisfy c² = rem */

            /* primitive check: gcd(a,b,c) == 1 */
            seq_t g = gcd((seq_t)a, (seq_t)b);
            g = gcd(g, (seq_t)c);
            if (g == (seq_t)1) ++count;
        }
    }

    return count;
}
