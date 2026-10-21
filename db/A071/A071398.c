/*
 * A071398  Rounded total surface area of a regular icosahedron with edge
 *          length n.
 * https://oeis.org/A071398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round(5 * n^2 * sqrt(3))
 * Range:     u8 n <= 5, u16 n <= 86, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root of a 128‑bit unsigned integer */
static inline unsigned __int128 isqrt128(unsigned __int128 v)
{
    unsigned __int128 lo = 0, hi = (unsigned __int128)1 << 64;   /* hi > sqrt(v) */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid * mid <= v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* x = 5 * n^2 */
    seq_t x = (seq_t)5 * (seq_t)n * (seq_t)n;

    /* compute S = floor(sqrt(3 * x^2)) */
    unsigned __int128 d = (unsigned __int128)3 * (unsigned __int128)x * (unsigned __int128)x;
    unsigned __int128 S128 = isqrt128(d);  /* S fits in 128 bits */
    seq_t S = (seq_t)S128;                 /* cast to seq_t (truncated if necessary) */

    /* decide whether to round up: 12 * x^2 >= (2*S + 1)^2 ? */
    unsigned __int128 left  = (unsigned __int128)12 * (unsigned __int128)x * (unsigned __int128)x;
    unsigned __int128 K     = (unsigned __int128)2 * S128 + 1;
    unsigned __int128 right = K * K;

    if (left >= right)
        return (S + 1);
    else
        return S;
}
