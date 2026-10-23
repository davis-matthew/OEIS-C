/*
 * A118657  a(n) = Sum_[k unrelated to n and k<n] a(k) = Sum_[k < n such that
 *          GCD(k,n) != 1 and k does not divide n ] a(k); a(1) = a(2) = a(3) =
 *          a(4) = 1.
 * https://oeis.org/A118657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 37, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=a(2)=a(3)=a(4)=1; for n>4,
   a(n)= Σ_{k=1}^{n-1} [ GCD(k,n)!=1 && k∤n ] * a(k) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t, seq_t) */

static seq_t *cache = NULL;      /* cache[i] holds a(i+1) */
static idx_t cache_cap = 0;      /* allocated capacity (1‑based) */
static idx_t cache_size = 0;     /* highest n computed so far */

/* Ensure cache has room for at least n elements (1‑indexed) */
static void ensure_cache_capacity(idx_t n)
{
    if (cache_cap >= n)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)   /* allocation failure */
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Pre‑compute terms up to n (inclusive) */
static void precompute(idx_t n)
{
    if (n <= cache_size)
        return;                      /* already done */
    ensure_cache_capacity(n);
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        seq_t val;
        if (i <= 4)                  /* base cases */
            val = 1;
        else
        {
            seq_t sum = 0;
            seq_t si   = (seq_t)i;   /* for gcd */
            for (idx_t j = 1; j < i; ++j)
            {
                if (i % j == 0)      /* k divides n → skip */
                    continue;
                seq_t sj = (seq_t)j;
                if (gcd(sj, si) != 1)  /* shares a prime factor */
                    sum += cache[j - 1];
            }
            val = sum;
        }
        cache[i - 1] = val;
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0, return 0 */
    precompute(n);
    return cache[n - 1];
}
