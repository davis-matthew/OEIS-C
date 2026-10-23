/*
 * A199855  Inverse permutation to A210521.
 * https://oeis.org/A199855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) =
 *            (2*j^2+(4*i-5)*j+2*i^2-3*i+2+(2+(-1)^j)*(1-(t+1)*(-1)^i))/4 with
 *            i = n - t*(t+1)/2, j = (t*t+3*t+4)/2 - n, t =
 *            floor((sqrt(8*n-7)-1)/2)
 * Range:     u8 n <= 235, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t low = 0;
    uint64_t high = (uint64_t)1 << 63;   /* enough for all possible sqrt values */
    while (low < high)
    {
        uint64_t mid = low + (high - low + 1) / 2;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* t = floor((sqrt(8*n-7) - 1) / 2) */
    unsigned __int128 val  = (unsigned __int128)8 * n - (unsigned __int128)7;
    uint64_t         sqrtv = isqrt_u128(val);
    int64_t          t     = (int64_t)((sqrtv - 1) / 2);

    /* i = n - t*(t+1)/2 ; j = (t^2+3*t+4)/2 - n */
    int64_t i = (int64_t)n - (int64_t)t * (t + 1) / 2;
    int64_t j = (t * t + 3 * t + 4) / 2 - (int64_t)n;

    /* parity factors for (-1)^i and (-1)^j */
    int s_i = (i & 1) ? -1 : 1;
    int s_j = (j & 1) ? -1 : 1;

    /* numerator of the formula (all using signed 128‑bit) */
    signed __int128 n1 = (signed __int128)2 * j * j;
    signed __int128 n2 = (signed __int128)(4 * i - 5) * j;
    signed __int128 n3 = (signed __int128)2 * i * i - (signed __int128)3 * i + (signed __int128)2;
    signed __int128 n4 = (signed __int128)(2 + s_j) * ((signed __int128)1 - (signed __int128)(t + 1) * s_i);
    signed __int128 num = n1 + n2 + n3 + n4;

    return (seq_t)(num / 4);
}
