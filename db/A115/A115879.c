/*
 * A115879  a(n) is the least positive x satisfying the Diophantine equation
 *          x^2=y(y+n). a(n)=0 if there are no solutions.
 * https://oeis.org/A115879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 520, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A115879: a(n) = least positive x such that x^2 = y(y+n) for some integer y.
   If no solution exists, a(n) = 0.
   The algorithm uses the identity
        (2y+n-2x)(2y+n+2x) = n^2
   and enumerates all factor pairs of n^2 that have the same parity
   and a difference divisible by 4.  For each valid pair it computes
   x = (b-a)/4 and keeps the minimal positive x. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>   /* for ULLONG_MAX */

/* Factorise m into primes p[i]^exp[i] (i=0..cnt-1).  m ≤ 2^64. */
static void factor(uint64_t m, idx_t fac[], idx_t exp[], size_t *cnt)
{
    size_t idx = 0;
    uint64_t d = m;
    for (uint64_t p = 2; p * p <= d; ++p)
    {
        if (d % p == 0)
        {
            size_t e = 0;
            while (d % p == 0) { d /= p; ++e; }
            fac[idx]   = (idx_t)p;
            exp[idx]   = (idx_t)e;
            ++idx;
        }
    }
    if (d > 1UL) { fac[idx] = (idx_t)d; exp[idx] = 1; ++idx; }
    *cnt = idx;
}

/* Recursively generate all divisors of n^2 using the exponents exp[i]
   (which are already doubled).  sz = number of prime factors. */
static void dfs(const idx_t fac[], const idx_t exp[], size_t sz,
                size_t pos, unsigned long long cur,
                unsigned long long n2, unsigned long long N,
                unsigned long long *best, bool *found)
{
    if (pos == sz)
    {
        unsigned long long d1 = cur;
        unsigned long long d2 = n2 / d1;
        if (d1 >= d2) return;             /* we need d1 < d2 */

        /* Same parity as n. */
        if ( (d1 & 1ULL) != (d2 & 1ULL) ) return;

        unsigned long long diff = d2 - d1;
        if (diff % 4ULL != 0ULL) return;   /* x must be integer */
        unsigned long long x = diff / 4ULL;
        if (x == 0ULL) return;             /* x must be positive */

        unsigned long long ynum = d1 + d2 - 2ULL * N;
        if (ynum % 4ULL != 0ULL) return;   /* y must be integer */

        if (!*found || x < *best)
        {
            *best  = x;
            *found = true;
        }
        return;
    }

    unsigned long long p = fac[pos];
    unsigned long long val = 1ULL;
    /* exp[pos] already contains the exponent of n^2 for this prime. */
    for (size_t e = 0; e <= exp[pos]; ++e)
    {
        dfs(fac, exp, sz, pos + 1, cur * val,
            n2, N, best, found);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Factor n.  (n ≤ 2^64 for the implementation.) */
    idx_t fac[64], exp[64];
    size_t cnt = 0;
    factor((uint64_t)n, fac, exp, &cnt);

    /* Prepare exponents for n^2 (doubled). */
    idx_t exp2[64];
    for (size_t i = 0; i < cnt; ++i) exp2[i] = 2 * exp[i];

    unsigned long long N   = (unsigned long long)n;
    unsigned long long n2  = N * N;
    unsigned long long best = ULLONG_MAX;
    bool found = false;

    dfs(fac, exp2, cnt, 0, 1ULL, n2, N, &best, &found);

    if (!found) return 0;
    return (seq_t)best;
}
