/*
 * A129296  Number of divisors of n^2 - 1 that are not greater than n.
 * https://oeis.org/A129296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n = 1, otherwise a(n) = τ(n²‑1) / 2 where τ(m) is
 *            the number of positive divisors of m
 * Range:     u8 n <= 4158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine below is fully self‑contained and uses only the
   utility function factor_u64.h to factor the numbers n‑1 and n+1.
   It works for any idx_t ≥ 1 and for all seq_t sizes supported by
   the build system. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

/* Merge the prime factorisation of v into the global arrays. */
static void merge_factor(uint64_t v,
                         uint64_t primes[], uint64_t exps[],
                         size_t *cnt)
{
    if (v <= 1) return;                /* 1 has no prime factors */

    uint64_t pbuf[64], ebuf[64];
    size_t cv = 0;
    factor_u64(v, pbuf, ebuf, &cv);    /* factor_u64 stores primes & exps */

    for (size_t i = 0; i < cv; ++i) {
        uint64_t p = pbuf[i];
        uint64_t e = ebuf[i];
        size_t j;

        for (j = 0; j < *cnt; ++j)
            if (primes[j] == p)
                break;

        if (j < *cnt)
            exps[j] += e;              /* existing prime – increase exponent */
        else {
            primes[*cnt] = p;
            exps[*cnt] = e;
            ++(*cnt);                  /* new prime – append */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;              /* definition from OEIS */

    uint64_t m1 = (uint64_t)n - 1;      /* n‑1  */
    uint64_t m2 = (uint64_t)n + 1;      /* n+1  */

    /* Because n²‑1 ≤ (n+1)² ≤ UINT64_MAX², the factors fit in uint64_t.
       The number of distinct primes in either factor is < 64. */
    static const unsigned MAX = 128;    /* safety margin for the combined list */
    uint64_t primes[MAX];
    uint64_t exps[MAX];
    size_t cnt = 0;

    merge_factor(m1, primes, exps, &cnt);
    merge_factor(m2, primes, exps, &cnt);

    seq_t divcnt = 1;
    for (size_t i = 0; i < cnt; ++i)
        divcnt *= (exps[i] + 1);        /* τ(m1·m2) = ∏(exp+1) */

    return divcnt / 2;                  /* a(n) = τ(n²‑1) / 2 */
}
