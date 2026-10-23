/*
 * A188260  [nr+kr]-[nr]-[kr], where r=sqrt(5), k=3, [ ]=floor.
 * https://oeis.org/A188260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n*sqrt(5)+3*sqrt(5)] - [n*sqrt(5)] - [3*sqrt(5)], where
 *            [x] denotes floor(x). Since [3*sqrt(5)] = 6, the simplified
 *            formula used is a(n) = floor((n+3)*sqrt(5)) - floor(n*sqrt(5)) -
 *            6. The implementation computes the two floor values with an
 *            integer square‑root via 128‑bit arithmetic
 * Range:     u8 n <= 135, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 128‑bit unsigned integer:
   returns floor(sqrt(x)). */
static inline unsigned __int128 isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1) << 64; /* exclusive upper bound */

    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 mid_sq = mid * mid;
        if (mid_sq <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Compute floor(n * sqrt(5))  == floor(sqrt(5 * n^2))  */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 n_sq = n128 * n128;
    unsigned __int128 term1 = n_sq * (unsigned __int128)5;
    unsigned __int128 f_n = isqrt_u128(term1);

    /* Compute floor((n+3) * sqrt(5)) == floor(sqrt(5 * (n+3)^2))  */
    unsigned __int128 n3 = n128 + (unsigned __int128)3;
    unsigned __int128 n3_sq = n3 * n3;
    unsigned __int128 term2 = n3_sq * (unsigned __int128)5;
    unsigned __int128 f_np3 = isqrt_u128(term2);

    /* a(n) = f_np3 - f_n - 6 */
    unsigned __int128 res = f_np3 - f_n - (unsigned __int128)6;
    return (seq_t)res;   /* 0 or 1, fits into any seq_t */
}
