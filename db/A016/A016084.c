/*
 * A016084  a(n+1) = a(n) + its digital root.
 * https://oeis.org/A016084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A016084:
   a(1) = 5
   a(n) = a(n-1) + digital_root(a(n-1))   for n > 1
   digital_root(x) = 1 + (x-1) % 9   (x > 0)   */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;     /* holds computed terms, indexed by n (1‑based) */
static size_t cap = 0;          /* allocated size for cache indices 0..cap */
static idx_t max_computed = 0;  /* largest index that has been computed */

/* One‑line digital–root function.  Works for any numeric seq_t. */
static inline seq_t digital_root(seq_t x)
{
    return (seq_t)1 + ((x - (seq_t)1) % (seq_t)9);
}

seq_t a(idx_t n)
{
    /* Offset is 1; gracefully handle n==0 by returning 0. */
    if (n == (idx_t)0)
        return 0;

    /* Initialise cache on first call. */
    if (!cache) {
        cap = 2;                                   /* space for indices 0,1 */
        cache = (seq_t *)calloc((size_t)cap + 1, sizeof(seq_t));
        if (!cache)
            return 0;                               /* allocation failed */
        cache[1] = (seq_t)5;                         /* a(1) */
        max_computed = 1;
    } else if ((size_t)n > cap) {
        /* Grow cache to accommodate index n; double until large enough. */
        size_t new_cap = (size_t)n;
        if (new_cap < 2)
            new_cap = 2;
        while (new_cap <= cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp)                                 /* allocation failed */
            return 0;
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = prev + digital_root(prev);
    }
    max_computed = n;
    return cache[n];
}
