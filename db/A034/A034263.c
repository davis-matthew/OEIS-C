/*
 * A034263  a(n) = binomial(n+4,4)*(4*n+5)/5.
 * https://oeis.org/A034263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n+4)*(4*n+5)/120
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 164, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t rem = 120;
    seq_t ans = 1;
    seq_t t, g;

    /* term n + 1 */
    t = (seq_t)n + 1;
    g = gcd(t, rem);
    t /= g;
    rem /= g;
    ans *= t;
    g = gcd(ans, rem);
    while (g > 1) {
        ans /= g;
        rem /= g;
        g = gcd(ans, rem);
    }

    /* term n + 2 */
    t = (seq_t)n + 2;
    g = gcd(t, rem);
    t /= g;
    rem /= g;
    ans *= t;
    g = gcd(ans, rem);
    while (g > 1) {
        ans /= g;
        rem /= g;
        g = gcd(ans, rem);
    }

    /* term n + 3 */
    t = (seq_t)n + 3;
    g = gcd(t, rem);
    t /= g;
    rem /= g;
    ans *= t;
    g = gcd(ans, rem);
    while (g > 1) {
        ans /= g;
        rem /= g;
        g = gcd(ans, rem);
    }

    /* term n + 4 */
    t = (seq_t)n + 4;
    g = gcd(t, rem);
    t /= g;
    rem /= g;
    ans *= t;
    g = gcd(ans, rem);
    while (g > 1) {
        ans /= g;
        rem /= g;
        g = gcd(ans, rem);
    }

    /* term 4*n + 5 */
    t = ((seq_t)n << 2) + 5;  /* 4*n + 5 */
    g = gcd(t, rem);
    t /= g;
    rem /= g;
    ans *= t;
    g = gcd(ans, rem);
    while (g > 1) {
        ans /= g;
        rem /= g;
        g = gcd(ans, rem);
    }

    return ans;
}
