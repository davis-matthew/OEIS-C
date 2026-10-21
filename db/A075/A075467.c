/*
 * A075467  Trajectory of 270798 under the Reverse and Add! operation carried
 *          out in base 4, written in base 10.
 * https://oeis.org/A075467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 270798; for n>0, a(n) = a(n-1) + reverse_base4(a(n-1))
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* reverse_base4(x) returns the decimal value of the base‑4 digits of x
   read in the reverse order. */

#include <stdlib.h>   /* malloc, realloc */

static seq_t reverse_base4(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        seq_t d = x % (seq_t)4;          /* lowest base‑4 digit */
        rev = rev * (seq_t)4 + d;        /* build reversed number */
        x = x / (seq_t)4;
    }
    return rev;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoization table */
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* initialise cache for the first term */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* unlikely, but guard */
        cache[0] = (seq_t)270798;
        cache_len = 1;
    }

    /* already computed? */
    if ((size_t)n < cache_len)
        return cache[n];

    /* grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* unlikely, but guard */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t rev   = reverse_base4(prev);
        cache[i] = prev + rev;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
