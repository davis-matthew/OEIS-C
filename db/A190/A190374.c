/*
 * A190374  a(n) = n + [n*r/t] + [n*s/t] + [n*u/t]; r=sin(Pi/5), s=1/r,
 *          t=sin(2*Pi/5), u=1/t.
 * https://oeis.org/A190374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*r/t⌋ + ⌊n*s/t⌋ + ⌊n*u/t⌋
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 128‑bit numbers */
static uint64_t isqrt128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 63, ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Scaling factor 2^SCALE to approximate sqrt(5) with high precision */
    static const int SCALE   = 50;             /* 2^50 ≈ 1.125e15 */
    static const uint64_t SCALE_VAL = 1ULL << SCALE;
    static bool init = false;
    static uint64_t sqrt5_scaled;   /* round( sqrt(5) * 2^SCALE ) */
    static uint64_t denom3;         /* 5*2^SCALE + sqrt5_scaled  */

    if (!init) {
        unsigned __int128 val = (unsigned __int128)5 * ((unsigned __int128)1 << (2 * SCALE));
        uint64_t s = isqrt128(val);
        /* round to the nearest integer */
        if ((unsigned __int128)(s + 1) * (s + 1) <= val)
            ++s;
        sqrt5_scaled = s;
        denom3 = (5ULL << SCALE) + sqrt5_scaled;
        init = true;
    }

    /* floor( n * r/t ) = floor( n * (sqrt5 - 2^SCALE) / (2*2^SCALE) ) */
    unsigned __int128 f1_num = (unsigned __int128)n * (sqrt5_scaled - SCALE_VAL);
    unsigned __int128 f1 = f1_num >> (SCALE + 1);

    /* floor( n * s/t ) = floor( n * (4*2^SCALE) / sqrt5_scaled ) */
    unsigned __int128 f2_num = (unsigned __int128)n * ((4ULL) << SCALE);
    unsigned __int128 f2 = f2_num / sqrt5_scaled;

    /* floor( n * u/t ) = floor( n * (8*2^SCALE) / denom3 ) */
    unsigned __int128 f3_num = (unsigned __int128)n * ((8ULL) << SCALE);
    unsigned __int128 f3 = f3_num / denom3;

    seq_t result = (seq_t)n + (seq_t)f1 + (seq_t)f2 + (seq_t)f3;
    return result;
}
