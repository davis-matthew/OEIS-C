/*
 * A160285  Numerator of Hermite(n, 23/29).
 * https://oeis.org/A160285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 23/29) * Recurrence: a(0)=1, a(1)=46, *
 *            a(n) = 46*a(n-1) - 1682*(n-1)*a(n-2) (n >= 2)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic array of computed terms */
static idx_t cache_size = 0;     /* number of elements already computed (last index + 1) */
static idx_t cache_cap  = 0;     /* allocated capacity (number of slots) */

/* Ensure that cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;

    idx_t new_cap = cache_cap ? cache_cap : 2;
    while (new_cap <= n)
        new_cap <<= 1;   /* double until large enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                /* memory allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* External linkage function required by the harness. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Compute missing values up to n, reusing cached ones. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)46;
        } else {
            seq_t term1 = (seq_t)46 * cache[i - 1];
            seq_t mul   = (seq_t)1682 * (seq_t)(i - 1);
            seq_t term2 = mul * cache[i - 2];
            cache[i] = term1 - term2;
        }
    }
    cache_size = n + 1;
    return cache[n];
}
