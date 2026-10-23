/*
 * A015207  Sum of Gaussian binomial coefficients for q=17.
 * https://oeis.org/A015207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 2, a(n) = 2*a(n-1) + a(n-2)*((17^(n-1)) - 1)
 *            for n >= 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;     /* stores a(n) values */
static seq_t *pow_cache = NULL;     /* stores 17^n values */
static size_t  cache_cap = 0;       /* allocated capacity */
static idx_t   computed = 0;        /* highest index computed */

seq_t a(idx_t n)
{
    /* Handle base cases directly, without initializing caches */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Lazily initialize cache for a(0) and a(1) */
    if (!seq_cache) {
        /* Ensure initial capacity for indices 0 and 1 */
        cache_cap = 2;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        pow_cache  = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)2;
        pow_cache[0] = (seq_t)1;
        pow_cache[1] = (seq_t)17;
        computed = 1;  /* a(0) and a(1) are now known */
    }

    /* Expand caches if necessary */
    if (n > computed) {
        size_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        pow_cache  = (seq_t *)realloc(pow_cache,  new_cap * sizeof(seq_t));
        cache_cap = new_cap;

        /* Compute new terms up to n */
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* pow_cache stores 17^(i-1) at index i-1 */
            seq_t pow   = pow_cache[i - 1];
            seq_t term  = pow - (seq_t)1;                 /* (17^(i-1)) - 1 */
            seq_t prod  = seq_cache[i - 2] * term;        /* a(i-2) * term */
            seq_t val   = ((seq_t)2) * seq_cache[i - 1];  /* 2 * a(i-1) */
            seq_cache[i] = val + prod;                   /* a(i) */
            pow_cache[i] = pow_cache[i - 1] * ((seq_t)17); /* 17^i for next step */
        }
        computed = n;
    }

    return seq_cache[n];
}
