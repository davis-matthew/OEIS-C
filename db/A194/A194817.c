/*
 * A194817  Number of integers k in [1,n] such that {n*r+k*r} < {n*r-k*r},
 *          where { } = fractional part and r=sqrt(3).
 * https://oeis.org/A194817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A194817:
   a(n) = #{k ∈ [1,n] :  {(n+k)*√3} < {(n-k)*√3} }.
   The inequality is equivalent to
      floor((n+k)√3) – floor((n-k)√3)  >  floor(2k√3).
   We compute all floor(·√3) values using integer square roots:
      floor(m√3) = floor( √(3*m*m) ).
   This avoids floating point arithmetic and runs in O(n) time
   for each n (here n ≤ 10 000). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer square root (floor) for values up to 1.2e9.
   Uses binary search on the limited range [0, 65536). */
static inline uint32_t isqrt_u64(uint64_t x)
{
    uint32_t lo = 0, hi = 65536U;          /* 65536² = 4 294 967 296 > 1.2e9 */
    while (lo < hi) {
        uint32_t mid = (lo + hi) >> 1;
        if ((uint64_t)mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* pre‑compute floor(m√3) for 0 ≤ m ≤ 2n */
    idx_t m_max = 2 * n;
    uint32_t *sq3 = (uint32_t *)malloc((m_max + 1) * sizeof(uint32_t));
    if (!sq3) return 0;                  /* allocation failure: return 0 */

    for (idx_t m = 0; m <= m_max; ++m) {
        uint64_t val = 3ULL * m * m;
        sq3[m] = isqrt_u64(val);
    }

    /* pre‑compute floor(2k√3) = floor(√(12k²)) for 0 ≤ k ≤ n */
    uint32_t *sq12 = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
    if (!sq12) { free(sq3); return 0; }

    for (idx_t k = 0; k <= n; ++k) {
        uint64_t val = 12ULL * k * k;
        sq12[k] = isqrt_u64(val);
    }

    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint32_t s = sq3[n + k] - sq3[n - k];   /* floor((n+k)√3) – floor((n−k)√3) */
        uint32_t g = sq12[k];                  /* floor(2k√3) */
        if (g <= s - 1) {
            ++count;
        }
    }

    free(sq3);
    free(sq12);
    return count;
}
