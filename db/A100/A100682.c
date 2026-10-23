/*
 * A100682  Floor of 4th root of pentatope numbers.
 * https://oeis.org/A100682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((C(n+3,4))^(1/4)) =
 *            floor((n*(n+1)*(n+2)*(n+3)/24)^(1/4))
 * Range:     u8 n <= 565, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer sqrt for __int128, returns floor(sqrt(x)) */
static __int128 isqrt128(__int128 x)
{
    __int128 lo = 0, hi = x;
    __int128 ans = 0;
    while (lo <= hi) {
        __int128 mid = (lo + hi) >> 1;
        __int128 sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* sequence term */
seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(n+3,4) = n*(n+1)*(n+2)*(n+3)/24 */
    __int128 a_val = n;
    __int128 b_val = n + 1;
    __int128 c_val = n + 2;
    __int128 d_val = n + 3;

    /* Divide by the factors of 24: 2,2,2,3 */
    const int factors[4] = {2, 2, 2, 3};
    for (int i = 0; i < 4; ++i) {
        int f = factors[i];
        if (a_val % f == 0)      a_val /= f;
        else if (b_val % f == 0) b_val /= f;
        else if (c_val % f == 0) c_val /= f;
        else if (d_val % f == 0) d_val /= f;
    }

    /* Now the product fits safely into __int128 */
    __int128 binom = a_val * b_val;
    binom *= c_val;
    binom *= d_val;

    /* Take integer fourth root: floor((binom)^(1/4)) */
    __int128 s = isqrt128(binom);        /* sqrt(binom) */
    __int128 m = isqrt128(s);            /* sqrt(sqrt(binom)) = fourth root */

    return (seq_t)m;
}
