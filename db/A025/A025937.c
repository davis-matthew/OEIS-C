/*
 * A025937  Expansion of 1/((1-2*x)*(1-3*x)*(1-6*x)*(1-7*x)).
 * https://oeis.org/A025937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n+1)-2^(n+1)+13*a(n‑1)-42*a(n‑2), a(0)=1, a(1)=18
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache previously computed values to answer calls in any order. */
static seq_t *cache   = NULL;
static idx_t cache_sz = 0;

/* Integer exponentiation by repeated squaring (works for both integer
   types and arbitrary‑precision classes).  The base and result are
   multiplied using the operators provided by seq_t.  */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & (idx_t)1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* OEIS A025937: expansion of 1/((1-2*x)(1-3*x)(1-6*x)(1-7*x)).  */
seq_t a(idx_t n)
{
    /* Initial terms are hard‑coded into the cache on first use. */
    if (cache_sz == 0)
    {
        cache_sz = 2;
        cache    = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)18;  /* a(1) */
    }

    if (n < cache_sz)
        return cache[n];

    /* Extend the cache to hold up to index n. */
    idx_t new_sz = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!new_cache) { free(cache); abort(); }
    cache = new_cache;

    /* Compute missing terms iteratively using the recurrence. */
    for (idx_t i = cache_sz; i <= n; ++i)
    {
        seq_t term = cache[i - 1] * (seq_t)13;
        term -= cache[i - 2] * (seq_t)42;
        term += pow_seq((seq_t)3, i + 1);
        term -= pow_seq((seq_t)2, i + 1);
        cache[i] = term;
    }

    cache_sz = new_sz;
    return cache[n];
}
