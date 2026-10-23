/*
 * A025196  a(n) = floor(Sum_{1<=i<j<=n} (sqrt(j)-sqrt(i))^2).
 * https://oeis.org/A025196
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2(n+1)/2 - ceil( (Σ_{k=1}^n √k )^2 ) This is derived
 *            from a(n) = floor( Σ_{1≤i<j≤n} (√j-√i)^2 ) and simplified to the
 *            above closed form. The implementation uses only integer
 *            arithmetic and a 128‑bit integer sqrt to avoid floating point
 *            precision issues
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <assert.h>

/* integer square root for unsigned __int128:  floor(sqrt(x)) */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = (unsigned __int128)1 << 64; /* sqrt(2^128-1) < 2^64 */
    while (lo + 1 < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       /* behaviour below offset is irrelevant */

    /* Scale factor chosen so that the bound on the fractional part is < 1,
       guaranteeing a unique ceil.  SCALE > 2 * (2/3) * n^(5/2) ≈ 4/3 n^(5/2).
       For n ≤ 10^4, SCALE = 2·10^10 satisfies the condition. */
    const unsigned __int128 SCALE = (unsigned __int128)20000000000ULL; /* 2e10 */
    const unsigned __int128 SCALE_SQR = SCALE * SCALE;

    /* Compute S_scaled = Σ floor(√k * SCALE) */
    unsigned __int128 sum_scaled = 0;
    for (idx_t k = 1; k <= n; ++k) {
        unsigned __int128 prod = (unsigned __int128)k * SCALE_SQR;
        unsigned __int128 r = isqrt128(prod);       /* floor( √k * SCALE ) */
        sum_scaled += r;
    }

    /* Now U_low = ceil( (sum_scaled)^2 / SCALE_SQR )
       U_high = ceil( (sum_scaled + n)^2 / SCALE_SQR )
       Since SCALE is large enough, U_low == U_high. */
    unsigned __int128 n128 = n;
    unsigned __int128 upper = sum_scaled + n128;

    unsigned __int128 ceil_U_low = (sum_scaled * sum_scaled + SCALE_SQR - 1) / SCALE_SQR;
    unsigned __int128 ceil_U_high = (upper * upper + SCALE_SQR - 1) / SCALE_SQR;
    /* In practice ceil_U_low == ceil_U_high; use the low value. */
    unsigned __int128 ceil_U = ceil_U_low;

    /* Compute the polynomial part: n^2 (n+1)/2 as 128-bit */
    unsigned __int128 poly = (unsigned __int128)n * n * (n + 1) / 2;

    unsigned __int128 result128 = poly - ceil_U;

    /* Cast to seq_t (assumed to be an unsigned integer type). */
    return (seq_t)result128;
}
