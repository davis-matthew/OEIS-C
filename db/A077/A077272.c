/*
 * A077272  a(n) = Sum_{d|n} d^2*2^(d-1)*(n/d-1) for n > 0.
 * https://oeis.org/A077272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} d² * 2^(d‑1) * (n/d - 1), a(0)=0 The
 *            implementation factorizes n, recursively enumerates all its
 *            divisors, evaluates the term for each divisor using only the
 *            core arithmetic operators and the shift operator (which works
 *            for any seq_t type, including arbitrary‑precision
 *            implementati...
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   dfs_divs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/dfs_divs.h"

/* Simple trial division factorisation of a 64‑bit integer.
   The result is stored in the arrays pr[] and exp[] and
   the number of distinct prime factors in cnt.  */
static void factor_uint64(uint64_t n, uint64_t *pr, int *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            int e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            pr[c] = p;
            exp[c] = e;
            ++c;
        }
    }
    if (n > 1)
    {
        pr[c] = n;
        exp[c] = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint64_t primes[20];
    int      exps[20];
    size_t   cnt;

    factor_uint64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    dfs_divs(0, 1, primes, exps, cnt, n, &result);
    return result;
}
