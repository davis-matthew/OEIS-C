/*
 * A195155  Number of divisors d of n such that d-1 also divides n or d-1 = 0.
 * https://oeis.org/A195155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{ d | d divides n and (d == 1 or n % (d - 1) == 0) }|
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Recursive helper to count qualifying divisors */
static void _dfs(size_t idx, idx_t cur, idx_t n,
                 const uint64_t *primes, const uint64_t *exps,
                 size_t pfsize, seq_t *cnt)
{
    if (idx == pfsize) {
        if (cur == (idx_t)1)
            (*cnt)++;
        else if (n % (cur - (idx_t)1) == (idx_t)0)
            (*cnt)++;
        return;
    }
    uint64_t p   = primes[idx];
    size_t   emax = (size_t)exps[idx];
    idx_t   val   = (idx_t)1;
    for (size_t e = 0; e <= emax; ++e) {
        _dfs(idx + 1, cur * val, n,
             primes, exps, pfsize, cnt);
        val = val * (idx_t)p;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n = 0 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t   pfsize;

    /* Factor n into primes and exponents */
    factor_u64((uint64_t)n, primes, exps, &pfsize);

    seq_t total = 0;
    _dfs(0, (idx_t)1, n,
         primes, exps, pfsize, &total);
    return total;
}
