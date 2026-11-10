/*
 * A082457  a(n) = gcd(1+2n, A057643(1+2n)).
 * https://oeis.org/A082457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(2n+1, A057643(2n+1)) = lcm{ gcd(2n+1, d+1) : d | 2n+1
 *            }
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static uint64_t A082457_gcd64(uint64_t x, uint64_t y)
{
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static void A082457_dfs(uint64_t m, const uint64_t *pr, const uint64_t *ex,
                        size_t cnt, size_t idx, uint64_t cur, uint64_t *acc)
{
    if (idx == cnt) {
        uint64_t g = A082457_gcd64(m, cur + 1);
        uint64_t h = A082457_gcd64(*acc, g);
        *acc = (*acc / h) * g;
        return;
    }
    uint64_t mult = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        A082457_dfs(m, pr, ex, cnt, idx + 1, cur * mult, acc);
        mult *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    uint64_t m = 2 * (uint64_t)n + 1;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);
    uint64_t acc = 1;
    A082457_dfs(m, pr, ex, cnt, 0, 1, &acc);
    return (seq_t)acc;
}
