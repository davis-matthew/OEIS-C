/*
 * A081001  n is in the sequence if and only if it does not rank among the top
 *          n positive integers in centrality (cf. A080997 for fuller
 *          explanation of this concept).
 * https://oeis.org/A081001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   n is a term iff at least n positive integers m != n have
 *            centrality A018804(m)/m^2 > A018804(n)/n^2. a(n) is the n-th
 *            such term, found by search, using A018804(m) = Product_{p^e ||
 *            m} p^(e-1)*((e+1)*p - e)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef unsigned __int128 u128_t;

static uint64_t *pil_tab = 0;
static uint64_t pil_cap = 0;

static uint64_t pil_calc(uint64_t m)
{
    uint64_t r = 1;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            uint64_t e = 0, pe = 1;
            while (m % p == 0)
            {
                m /= p;
                ++e;
                if (e > 1) pe *= p;
            }
            r *= pe * ((e + 1) * p - e);
        }
    }
    if (m > 1) r *= (2 * m - 1);
    return r;
}

static uint64_t pil_get(uint64_t m)
{
    if (m >= pil_cap)
    {
        uint64_t ncap = pil_cap ? pil_cap * 2 : 1024;
        while (ncap <= m) ncap *= 2;
        uint64_t *nt = (uint64_t *)realloc(pil_tab, ncap * sizeof(uint64_t));
        if (!nt) return pil_calc(m);
        for (uint64_t i = pil_cap; i < ncap; ++i) nt[i] = 0;
        pil_tab = nt;
        pil_cap = ncap;
    }
    if (pil_tab[m] == 0) pil_tab[m] = pil_calc(m);
    return pil_tab[m];
}

/* true if n is NOT among the top n in centrality (term of A081001) */
static bool in_seq(uint64_t k)
{
    uint64_t Pk = pil_get(k);
    u128_t k2 = (u128_t)k * k;
    u128_t bound = 4 * k2 * k2;
    u128_t Pk2 = (u128_t)Pk * Pk;
    uint64_t count = 0;
    for (uint64_t m = 1; (u128_t)m * Pk2 < bound; ++m)
    {
        if (m == k) continue;
        u128_t lhs = (u128_t)pil_get(m) * k2;
        u128_t rhs = (u128_t)Pk * m * m;
        if (lhs > rhs)
        {
            if (++count >= k) return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    uint64_t found = 0, k = 0;
    while (found < (uint64_t)n)
    {
        ++k;
        if (in_seq(k)) ++found;
    }
    return (seq_t)k;
}
