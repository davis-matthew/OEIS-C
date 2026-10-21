/*
 * A054529  Number of words with 3 runs in Varshamov-Tenengolts code VT_0(n)
 *          of length n.
 * https://oeis.org/A054529
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   * For a word of length n over {0,1} with 3 runs, let * l1 , l2 ,
 *            l3 >= 1 with l1+l2+l3 = n * and start bit b ∈ {0,1}. The
 *            weighted sum modulo M=n+1 is * S = b · B1 + (1-b) · B2 (mod M),
 *            * where * B1 = l1(l1+1)/2 + n(n+1)/2 – s(s+1)/2 * B2 = s(s+1)/2
 *            – l1(l1+1)/2 * s = l1 + l2. * A word belongs to...
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Valid for n ≥ 3 only; results for smaller n are irrelevant. */
    if (n < 3)
        return (seq_t)0;

    /* M = n+1 is small (≤10001).  All intermediate arithmetic fits in 64‑bit. */
    idx_t M      = n + 1;
    uint64_t M64 = (uint64_t)M;

    /* Pre‑compute total weighted sum modulo M. */
    uint64_t T_mod = ((uint64_t)n * (n + 1) / 2) % M64;

    uint64_t ans = 0;

    for (idx_t l1 = 1; l1 <= n - 2; ++l1)
    {
        uint64_t sum_l1 = (uint64_t)l1 * (l1 + 1) / 2;
        for (idx_t l2 = 1; l2 <= n - l1 - 1; ++l2)
        {
            idx_t      l3 = n - l1 - l2;          /* automatically ≥ 1 */
            uint64_t sum_s    = (uint64_t)(l1 + l2) * (l1 + l2 + 1) / 2;

            /* B2 = s(s+1)/2 – l1(l+1)/2  (mod M) */
            uint64_t B2 = (sum_s % M64 + M64 - sum_l1 % M64) % M64;

            /* B1 = total – B2  (mod M) */
            uint64_t B1 = (T_mod + M64 - B2) % M64;

            if (B2 == 0) ++ans;   /* start bit b = 0 satisfies */
            if (B1 == 0) ++ans;   /* start bit b = 1 satisfies */
        }
    }

    return (seq_t)ans;
}
