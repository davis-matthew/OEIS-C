/*
 * A169442  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = b(n) + 43*(a(n-1)+…+a(n-31)) – 946*a(n-32)
   where     b(0)=1,  b(1..31)=2,  b(32)=1,  b(n)=0 for n>32.
   Initial terms: a(0)=1, a(1)=45, a(2)=1980, … (as in OEIS). */

#include <stddef.h>
#include <stdlib.h>

/* dynamic cache of computed terms */
static seq_t *cache = NULL;      /* cache[i] stores a(i) */
static size_t cache_len = 0;     /* number of terms stored (cache_len > 0 after init) */
static size_t cache_cap = 0;     /* allocated capacity of cache array */

/* running sum of the last up to 31 terms: sum_{j=1}^{31} a(n-j) for the current n */
static seq_t sumPrev31 = 0;

/* ensure cache contains a(i) for all 0 <= i <= n */
static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n;
    if (cache_len == 0) {
        /* initial allocation */
        cache_cap = target + 1;
        if (cache_cap < 8) cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = 1;            /* a(0) */
        cache_len = 1;
        sumPrev31 = cache[0];    /* for n=1 we need a(0) */
        return;
    }

    if (target < cache_len) return;  /* already computed */

    /* grow capacity if needed */
    while (cache_cap <= target) cache_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    if (!tmp) return;  /* allocation failure: abort silently (should not happen) */
    cache = tmp;

    /* compute new terms up to n */
    for (size_t i = cache_len; i <= target; ++i) {
        seq_t b;
        if (i == 0)          b = 1;
        else if (i <= 31)    b = 2;
        else if (i == 32)    b = 1;
        else                b = 0;

        seq_t term = b + ((seq_t)43) * sumPrev31;
        if (i >= 32) term -= ((seq_t)946) * cache[i - 32];

        cache[i] = term;

        /* update running sum for next iteration */
        sumPrev31 += term;
        if (i >= 31) sumPrev31 -= cache[i - 31];
    }
    cache_len = target + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
