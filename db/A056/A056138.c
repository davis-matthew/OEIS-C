/*
 * A056138  Number of ways in which n can be the shorter leg (shortest side)
 *          of an integer-sided right triangle.
 * https://oeis.org/A056138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer m > n such that n² + m² is a perfect
 *            square. Computed by enumerating divisor pairs (u,v) of n² with u
 *            < v, same parity, and checking if (v–u)/2 > n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9502 (2026-11-01)
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_uint64.h"

/* Recursive helper to generate all divisors of n² up to n. */
static void dfs_div(uint64_t cur, int idx, int cnt, uint64_t N, uint64_t N2,
                    uint64_t fac[], int exp[], uint64_t *ans)
{
    if (cur > N) return;                 /* pruning: product already too large */
    if (idx == cnt) {                    /* reached a divisor */
        if (cur < N) {
            uint64_t v = N2 / cur;        /* complementary divisor */
            if (((cur & 1u) == (v & 1u))) {
                uint64_t m = (v - cur) / 2u;
                if (m > N) (*ans)++;
            }
        }
        return;
    }

    uint64_t p = fac[idx];
    uint64_t cur2 = cur;
    int maxe = 2 * exp[idx];              /* exponent range for n² */
    for (int e = 0; e <= maxe; ++e) {
        if (cur2 > N) break;              /* further multiplication makes it larger */
        dfs_div(cur2, idx + 1, cnt, N, N2, fac, exp, ans);
        cur2 *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t N  = (uint64_t)n;
    uint64_t N2 = N * N;

    uint64_t fac[20];
    int exp[20];
    int cnt;
    factor_uint64(N, fac, exp, &cnt);

    uint64_t ans = 0;
    dfs_div(1, 0, cnt, N, N2, fac, exp, &ans);
    return (seq_t)ans;
}
