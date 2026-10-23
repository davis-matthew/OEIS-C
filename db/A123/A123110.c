/*
 * A123110  Triangle T(n,k), 0 <= k <= n, read by rows given by
 *          [0,1,0,0,0,0,0,0,0,0,...] DELTA [1,0,-1,1,0,0,0,0,0,0,...] where
 *          DELTA is the operator defined in A084938.
 * https://oeis.org/A123110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 for n = 0 or n not a positive triangular number; a(n) =
 *            0 if n > 0 is a triangular number. Equivalently, a(n) = 1 -
 *            A010054(n)
 * Range:     u8 n <= 9466, u16 n <= 9466, u32 n <= 9466, u64 n <= 9466,
 *            u128 n <= 9466, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, etc. */

static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1) << 34; /* 2^34 > sqrt(8*2^64+1) */
    while (lo <= hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;   /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    unsigned __int128 t = ((unsigned __int128)n) * 8 + 1;   /* 8*n + 1 */
    unsigned __int128 s = isqrt128(t);

    if (s * s == t && (s & 1))   /* integer sqrt is odd => n is triangular */
        return (seq_t)0;
    else
        return (seq_t)1;
}
