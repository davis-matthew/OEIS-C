/*
 * A186643  The number of divisors d of n which are either d=1 or for which
 *          the highest power d^k dividing n has odd exponent k.
 * https://oeis.org/A186643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n such that * either d = 1 or,
 *            letting k be the largest integer with d^k | n, * k is odd
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: factor n, enumerate all divisors by recursion,
 *  and test the condition on each. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static void dfs(int pos, int cnt, const uint64_t exponents[],
                int f[], bool any_nonzero, seq_t *res)
{
    if (pos == cnt) {
        if (!any_nonzero) {
            (*res)++;
            return;
        }
        int min_k = -1;
        for (int i = 0; i < cnt; ++i) {
            int fi = f[i];
            if (fi > 0) {
                int k = (int)(exponents[i] / fi);
                if (min_k == -1 || k < min_k) {
                    min_k = k;
                }
            }
        }
        if ((min_k & 1) == 1) {
            (*res)++;
        }
        return;
    }
    for (int e = 0; e <= (int)exponents[pos]; ++e) {
        f[pos] = e;
        dfs(pos + 1, cnt, exponents, f, any_nonzero || (e > 0), res);
    }
}

seq_t a(idx_t n)
{
    /* Special case n == 0 is outside OEIS offset; return 0. */
    if (n == 0) return 0;

    uint64_t primes[64];
    uint64_t exponents[64];
    size_t pcnt = 0;

    /* Factor n using the provided utility function. */
    factor_u64((uint64_t)n, primes, exponents, &pcnt);

    int f[64] = {0};
    seq_t result = 0;
    dfs(0, (int)pcnt, exponents, f, false, &result);
    return result;
}
