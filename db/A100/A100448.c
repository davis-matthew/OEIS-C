/*
 * A100448  Number of triples (i,j,k) with 1 <= i <= j < k <= n and gcd{i,j,k}
 *          = 1.
 * https://oeis.org/A100448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6) * (-1 + Σ_{k=1..n} μ(k) * ⌊n/k⌋³)
 * Range:     u8 n <= 12, u16 n <= 77, u32 n <= 3140, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9135 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS A100448: number of triples (i,j,k) with 1 ≤ i ≤ j < k ≤ n and gcd(i,j,k)=1. */

static int mobius_uint64(uint64_t n) {
    int mu = 1;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            temp /= p;
            if (temp % p == 0) return 0;          /* squared prime factor */
            mu = -mu;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) mu = -mu;                       /* last prime factor */
    return mu;
}

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
