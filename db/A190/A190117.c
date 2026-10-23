/*
 * A190117  a(n) = Sum_{k=1..n} k*k', where n' is the arithmetic derivative of
 *          n.
 * https://oeis.org/A190117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k*k' , where k' is the arithmetic derivative
 *            of k
 * Range:     u8 n <= 9, u16 n <= 63, u32 n <= 2555, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------------
   Smallest-prime-factor sieve.  All functions and variables below are
   static; the function `a` is the only externally visible entry point.
   -------------------------------------------------------------------------- */

static uint64_t *spf = NULL;          /* spf[i] = smallest prime dividing i   */
static idx_t   spf_limit = 0;         /* largest n for which spf is built     */

static void init_spf(idx_t limit)
{
    spf = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = i;
            if (i <= limit / i)            /* avoid overflow when computing i*i */
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
        }
    }
    spf_limit = limit;
}

/* Compute the contribution of a single k to the sum:
   k * k' = k^2 * Σ(e/p) over prime factors p^e of k */
static seq_t contribution(idx_t k)
{
    if (k <= 1) return 0;
    seq_t k_sq = (seq_t)k * (seq_t)k;
    seq_t sum = 0;
    idx_t temp = k;
    while (temp > 1) {
        uint64_t p = spf[temp];
        uint64_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }
        /* each term k^2/p is an integer */
        seq_t term = (k_sq / (seq_t)p) * (seq_t)exp;
        sum += term;
    }
    return sum;
}

/* Cache for already computed a(n) values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;           /* allocated size of cache */
static idx_t  cache_last = 0;          /* highest n for which cache[n] is valid */

static void ensure_cache(idx_t n)
{
    if (cache == NULL || cache_cap < n + 2) {    /* +2 for safety */
        cache_cap = n + 2;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        cache[0] = 0;
    }
}

/* --------------------------------------------------------------------------
   Public function: computes a(n) for n >= 1.  Calls may be in any order.
   -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not part of the OEIS offset */

    ensure_cache(n);

    /* Build spf up to the maximum requested n if needed */
    if (n > spf_limit) init_spf(n);

    /* Incrementally compute values up to n if not already done */
    if (cache_last < n) {
        for (idx_t k = cache_last + 1; k <= n; ++k) {
            seq_t contrib = contribution(k);
            cache[k] = cache[k - 1] + contrib;
        }
        cache_last = n;
    }
    return cache[n];
}
