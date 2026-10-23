/*
 * A033737  Expansion of Product_{d | 25} theta_3(q^d).
 * https://oeis.org/A033737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a,b,c∈ℤ, a²+5b²+25c²=n} 2^{[a≠0]+[b≠0]+[c≠0]}
 *            (coefficient of qⁿ in ∏_{d|25} θ₃(qᵈ), where
 *            θ₃(q)=1+2∑_{k≥1}q^{k²})
 * Range:     u8 n <= 5880, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root (floor) for 64‑bit unsigned integers – fast enough for n≤10 000 */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;
    /* iterate over c (quadratic coeff 25) */
    idx_t maxC = isqrt_u64(n / 25);
    for (idx_t c = 0; c <= maxC; ++c) {
        seq_t wC = (c == 0 ? 1 : 2);
        idx_t term25 = (idx_t)((__uint128_t)25 * c * c);
        idx_t rem1  = n - term25;

        /* iterate over b (quadratic coeff 5) */
        idx_t maxB = isqrt_u64(rem1 / 5);
        for (idx_t b = 0; b <= maxB; ++b) {
            seq_t wB = (b == 0 ? 1 : 2);
            idx_t term5 = (idx_t)((__uint128_t)5 * b * b);
            idx_t rem2  = rem1 - term5;

            idx_t sq = isqrt_u64(rem2);
            if (sq * sq == rem2) {
                seq_t wA = (sq == 0 ? 1 : 2);
                count += wA * wB * wC;
            }
        }
    }
    return count;
}
