/*
 * A051162  Triangle T(n,k) = n+k, n >= 0, 0 <= k <= n.
 * https://oeis.org/A051162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice, look
 * Formula:   a(n) = (t - t^2 + 2*(n+1) - 2)/2 where t =
 *            floor((sqrt(8*(n+1)-7)-1)/2), a(0)=0 Implementation uses a
 *            binary‑search integer sqrt on an unsigned __int128 value and
 *            128‑bit arithmetic to avoid overflow
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of an unsigned __int128, result fits in 64 bits */
static inline unsigned long long isqrt128(unsigned __int128 x)
{
    unsigned long long lo = 0, hi = 0xFFFFFFFFFFFFFFFFULL, ans = 0;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* use unsigned __int128 for intermediate calculations */
    unsigned __int128 m128 = (unsigned __int128)n + 1;        /* m = n+1              */
    unsigned __int128 val = 8 * m128 - 7;                    /* sqrt argument        */
    unsigned long long s = isqrt128(val);                     /* floor sqrt(val)      */
    unsigned long long t = (s - 1) / 2;                      /* t = floor((s-1)/2)   */

    /* compute (t - t^2 + 2*m - 2)/2 with 128‑bit signed arithmetic */
    __int128 two_m_minus2 = (__int128)((unsigned __int128)2 * m128 - 2);
    __int128 lhs = (__int128)t - (__int128)t * (__int128)t + two_m_minus2;
    seq_t result = (seq_t)(lhs / 2);

    return result;
}
