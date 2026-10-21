/*
 * A073834  Denominators of b(n) where b(1) = 1, b(i) = b(i-1) + 1/b(i-1).
 * https://oeis.org/A073834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, changed
 * Formula:   a(n) = denominator of b(n) where b(1) = 1, b(k) = b(k-1) +
 *            1/b(k-1) (fraction reduced)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The sequence element type (seq_t) and index type (idx_t)
   are defined by the build system.  seq_t supports
   +, -, *, /, % and comparison operators. */
static seq_t *den_cache = NULL;   /* cache of denominators */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity */
static idx_t last_computed = 1;   /* highest n for which cache is valid */
static seq_t num = (seq_t)1;      /* numerator of current b(n)   */
static seq_t den = (seq_t)1;      /* denominator of current b(n) */

/* Expand the cache to hold at least 'needed' terms.
   Uses realloc to grow the array geometrically. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(den_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);          /* no memory -> abort */
    den_cache = new_ptr;
    cache_cap = new_cap;
}

/* Compute a(n) – denominators of the sequence A073834. */
seq_t a(idx_t n)
{
    if (n == 0)                         /* offset is 1, but guard anyway */
        return (seq_t)0;
    if (den_cache == NULL) {            /* first call: initialise cache */
        grow_cache(1);
        den_cache[0] = (seq_t)1;        /* a(1) = 1 */
        cache_len = 1;
        num = (seq_t)1;
        den = (seq_t)1;
        last_computed = 1;
    }

    /* If the requested term is already cached, return it. */
    if (n <= last_computed)
        return den_cache[n - 1];

    /* Extend cache up to n. */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        seq_t new_num = num * num + den * den; /* (p^2 + q^2) */
        seq_t new_den = num * den;            /* p * q      */

        num = new_num;
        den = new_den;

        ++last_computed;
        grow_cache(last_computed);
        den_cache[last_computed - 1] = new_den;
    }

    return den_cache[n - 1];
}
