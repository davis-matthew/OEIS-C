/*
 * A108673  a(n) = (n+1)(n+2)(n+3)(2n+3)(10n^2 + 27n + 20)/360.
 * https://oeis.org/A108673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)(n+2)(n+3)(2n+3)(10n^2 + 27n + 20)/360
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean gcd for unsigned 64‑bit values.  Used to cancel the
   common factors with the fixed denominator 360. */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Work with 64‑bit intermediates to avoid overflow for
       arbitrary n, even when seq_t may be as small as 8 bits. */
    uint64_t N   = (uint64_t)n;
    uint64_t f[5];

    f[0] = N + 1;                       /* n+1 */
    f[1] = N + 2;                       /* n+2 */
    f[2] = N + 3;                       /* n+3 */
    f[3] = N * 2 + 3;                   /* 2n+3 */
    f[4] = N * N * 10 + N * 27 + 20;    /* 10n^2+27n+20 */

    uint64_t denom = 360;
    for (int i = 0; i < 5; ++i) {
        uint64_t g = gcd_uint64(f[i], denom);
        if (g > 1) {
            f[i]   /= g;
            denom  /= g;
        }
    }
    /* denom should be 1 after full cancellation */

    seq_t res = 1;
    for (int i = 0; i < 5; ++i) {
        res *= (seq_t)f[i];
    }
    return res;
}
