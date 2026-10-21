/*
 * A080226  Number of deficient divisors of n.
 * https://oeis.org/A080226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{d | d divides n and σ(d) < 2·d}|, where σ(d) is the sum
 *            of the divisors of d
 * Range:     u8 n <= 8944, u16 n <= 8944, u32 n <= 8944, u64 n <= 8944,
 *            u128 n <= 8944, big any n
 * Helpers:   dfs_deficient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/dfs_deficient.h"

/* ------------------------------------------------------------------
   Simple trial division factorisation for 64‑bit numbers.
   ------------------------------------------------------------------ */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, int *cnt)
{
    *cnt = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            uint64_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            pr[*cnt] = p;
            exp[*cnt] = e;
            ++(*cnt);
        }
    }
    if (tmp > 1) /* remaining prime factor */
    {
        pr[*cnt] = tmp;
        exp[*cnt] = 1;
        ++(*cnt);
    }
}

/* ------------------------------------------------------------------
   Main sequence function.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* behaviour below the offset is irrelevant */

    uint64_t N = (uint64_t)n;

    uint64_t primes[64];
    uint64_t exps[64];
    int cnt;
    factor_u64(N, primes, exps, &cnt);

    seq_t res = 0;
    dfs_deficient(primes, exps, 0, cnt, 1, (unsigned __int128)1, &res);
    return res;
}
