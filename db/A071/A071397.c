/*
 * A071397  Rounded total surface area of a regular dodecahedron with edge
 *          length n.
 * https://oeis.org/A071397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round(3 * n^2 * sqrt(25 + 10*sqrt(5)))
 * Range:     u8 n <= 3, u16 n <= 56, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute integer sqrt (floor) of an unsigned __int128 */
static unsigned __int128 isqrt_u128(unsigned __int128 x) {
    unsigned __int128 lo = 0, hi = (unsigned __int128)1 << 64; /* hi > sqrt(x) for all x <= 2^128-1 */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* nearest integer to sqrt(x); x fits in 128-bit */
static unsigned __int128 round_isqrt_u128(unsigned __int128 x) {
    unsigned __int128 t = isqrt_u128(x);           /* floor sqrt */
    unsigned __int128 diff = x - t * t;            /* 0 <= diff < 2*t+1 */
    if (diff * 2 >= (t << 1) + 1)                  /* round up if closer to next integer */
        t += 1;
    return t;
}

/* Core function, returns sequence value */
seq_t a(idx_t n)
{
    /* Use a large scaling factor to guarantee correct rounding.
       Scale = 2^35 is enough for n up to at least 10^5. */
    static const unsigned __int128 SCALE = (unsigned __int128)1 << 35;

    /* Precompute constants once */
    static int init = 0;
    static unsigned __int128 sqrt5_num;   /* floor(sqrt(5) * SCALE) */
    static unsigned __int128 s_scaled;    /* round(sqrt(25 + 10*sqrt(5)) * SCALE) */

    if (!init) {
        /* sqrt5_num = floor(sqrt(5) * SCALE) */
        sqrt5_num = isqrt_u128((unsigned __int128)5 * SCALE * SCALE);

        /* Compute X = (25 * SCALE^2 + 10 * sqrt5_num * SCALE) */
        unsigned __int128 X = (unsigned __int128)25 * SCALE * SCALE
                           + (unsigned __int128)10 * sqrt5_num * SCALE;

        /* s_scaled = round( sqrt(X) ) */
        s_scaled = round_isqrt_u128(X);

        init = 1;
    }

    if (n == 0)
        return (seq_t)0;

    unsigned __int128 un = n;                     /* cast to 128-bit for safety */
    unsigned __int128 val = (unsigned __int128)3 * un * un * s_scaled;
    /* round (val / SCALE) : add SCALE/2 before division */
    unsigned __int128 res = (val + (SCALE >> 1)) / SCALE;
    return (seq_t)res;
}
