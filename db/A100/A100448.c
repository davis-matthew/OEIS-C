/*
 * A100448  Number of triples (i,j,k) with 1 <= i <= j < k <= n and gcd{i,j,k}
 *          = 1.
 * https://oeis.org/A100448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6) * (-1 + Σ_{k=1..n} μ(k) * ⌊n/k⌋³)
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   mobius_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS A100448: number of triples (i,j,k) with 1 ≤ i ≤ j < k ≤ n and gcd(i,j,k)=1. */
#include "../utils/mobius_uint64.h"

seq_t a(idx_t n)
{
    /* compute S = Σ μ(k) * floor(n/k)^3 */
    int64_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        int mu = mobius_uint64(k);          /* μ(k) ∈ {–1, 0, 1} */
        if (mu == 0) continue;
        idx_t m = n / k;                    /* floor(n/k) */
        int64_t m3 = (int64_t)m * m * m;    /* m³ fits in 64‑bit for n ≤ 10000 */
        sum += mu * m3;
    }
    /* a(n) = (S - 1) / 6 */
    int64_t result_signed = (sum - 1) / 6;
    return (seq_t)result_signed;
}
