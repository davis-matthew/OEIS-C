/*
 * A025197  a(n) = floor( Sum_{1 <= i < j <= n} ((sqrt(j)-sqrt(i))^3) ).
 * https://oeis.org/A025197
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Sum_{1 <= i < j <= n} ((sqrt(j)-sqrt(i))^3) )
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline __int128 isqrt128(__int128 x)
{
    if (x <= 0) return 0;
    __int128 r = 1;
    while (r <= x / r) r <<= 1;
    __int128 low = r >> 1, high = r;
    while (low < high) {
        __int128 mid = (low + high + 1) >> 1;
        if (mid <= x / mid) low = mid;
        else high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    const int SCALE = 55;                 /* fractional bits for fixed‑point */
    const unsigned __int128 SCALE_FACTOR = ((unsigned __int128)1) << SCALE;

    /* Precompute sqrt(1) in fixed point */
    __int128 sqrt1_scaled = isqrt128((( __int128)1) << (2 * SCALE));
    __int128 sumSqrt   = sqrt1_scaled;          /* Σ sqrt(i) for i < current k   */
    __int128 sumISqrt  = sqrt1_scaled;          /* Σ i*sqrt(i) for i < current k */

    __int128 acc = 0;   /* accumulated scaled sum S(n) */

    for (idx_t k = 2; k <= n; ++k) {
        /* Values for indices < k */
        __int128 preSumSqrt   = sumSqrt;
        __int128 preSumISqrt  = sumISqrt;
        __int128 preSumI      = (__int128)(k - 1) * k / 2;   /* Σ i for i<k */

        /* sqrt(k) in fixed point */
        __int128 sqrtk_scaled = isqrt128((( __int128)k) << (2 * SCALE));

        /* D(k) in fixed point */
        __int128 term1 = (__int128)(k - 1) * k * sqrtk_scaled;           /* (k-1)*k*sqrt(k)   */
        __int128 term2 = -(__int128)(3 * k) * preSumSqrt;                /* -3*k*Σ sqrt(i)    */
        __int128 term3 = (__int128)3 * sqrtk_scaled * preSumI;           /* +3*sqrt(k)*Σ i    */
        __int128 term4 = -preSumISqrt;                                  /* -Σ i*sqrt(i)      */

        __int128 D_scaled = term1 + term2 + term3 + term4;
        acc += D_scaled;

        /* Update running sums for next k */
        sumSqrt += sqrtk_scaled;
        sumISqrt += (__int128)k * sqrtk_scaled;
    }

    /* Convert back from fixed point and return the floor */
    seq_t result = (seq_t)(((unsigned __int128)acc) >> SCALE);
    return result;
}
