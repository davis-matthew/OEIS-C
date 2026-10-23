/*
 * A178100  Let B_m be set of divisors 1<=d<=m/2 of binomial(m-d-1,d-1) such
 *          that gcd(m,d)>1. The sequence lists m for which the intersection
 *          of B_m and B_(m+1) is not empty.
 * https://oeis.org/A178100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A178100 – Sequence of m such that Bₘ ∩ Bₘ₊₁ ≠ ∅.
    B_n is the set of divisors d (1 ≤ d ≤ n/2) with gcd(n,d)>1 that divide
    C(n‑d‑1, d‑1).  The function searches successive m, testing whether
    some divisor d belongs to both B_m and B_{m+1}.  Results are cached
    so that repeated calls are fast.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Euclidean GCD for 64‑bit values. */
static uint64_t ugcd(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* vp_factorial(n,p): exponent of prime p in n! (Legendre). */
static uint64_t vp_factorial(uint64_t n, uint64_t p)
{
    uint64_t v = 0;
    while (n) {
        n /= p;
        v += n;
    }
    return v;
}

/* Checks whether the divisor d divides both binom(m‑d‑1,d‑1) and
   binom(m‑d,d‑1).  The test uses p‑adic valuation. */
static bool divides_both(uint64_t m, uint64_t d)
{
    uint64_t t1 = m - d - 1;    /* binom(t1, d-1)  */
    uint64_t t2 = m - d;        /* binom(t2, d-1)  */
    uint64_t r  = d - 1;

    uint64_t x = d;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p != 0) continue;
        int e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        uint64_t v1 = vp_factorial(t1, p) - vp_factorial(r, p) - vp_factorial(t1 - r, p);
        if (v1 < (uint64_t)e) return false;
        uint64_t v2 = vp_factorial(t2, p) - vp_factorial(r, p) - vp_factorial(t2 - r, p);
        if (v2 < (uint64_t)e) return false;
    }
    if (x > 1) {               /* x is a prime factor with exponent 1 */
        uint64_t p = x;
        int e = 1;
        uint64_t v1 = vp_factorial(t1, p) - vp_factorial(r, p) - vp_factorial(t1 - r, p);
        if (v1 < (uint64_t)e) return false;
        uint64_t v2 = vp_factorial(t2, p) - vp_factorial(r, p) - vp_factorial(t2 - r, p);
        if (v2 < (uint64_t)e) return false;
    }
    return true;
}

/* Checks whether a given m is in the sequence. */
static bool m_satisfies(uint64_t m)
{
    uint64_t limit = m / 2;
    for (uint64_t d = 2; d <= limit; ++d) {
        if (ugcd(m, d) <= 1)          continue;
        if (ugcd(m + 1, d) <= 1)      continue;
        if (divides_both(m, d))       return true;
    }
    return false;
}

/* Cache for previously computed terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static uint64_t next_m    = 1;

/* Grow cache storage. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) abort();
    cache = p;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* invalid index – not used */
    
    while (cache_size < (size_t)n) {
        if (cache_cap <= cache_size) grow_cache();
        if (m_satisfies(next_m))
            cache[cache_size++] = (seq_t)next_m;
        ++next_m;
    }
    return cache[n - 1];
}
