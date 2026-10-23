/*
 * A128058  Expansion of 1/((1-x)sqrt(1-2x+5x^2)).
 * https://oeis.org/A128058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = sum_{k=0..n} sum_{j=0..k} C(k,j)*C(k-j,j)*(-1)^j
 *            Recurrence (n ≥ 3): n*a(n) + (-3*n+1)*a(n-1) + (7*n-6)*a(n-2) +
 *            5*(-n+1)*a(n-3) = 0 Initial values: a(0)=1, a(1)=2, a(2)=1
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 25, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Static memoisation cache */
static seq_t *cache = NULL;              /* array of stored terms */
static idx_t  cache_size = 0;            /* number of terms currently stored (max index + 1) */
static size_t cache_capacity = 0;        /* allocated capacity */

/* Ensure the cache can store at least up to index ‘n’ */
static void ensure_cache(idx_t n)
{
    if (n < (idx_t)cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                /* allocation failed – abort */
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_capacity = new_cap;
}

/* Primary sequence function */
seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;

    /* Ensure cache can hold all indices up to n */
    ensure_cache(n);

    /* Initialise base values if this is the first computation */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        cache_size = 3;
    }

    /* If the requested value is already cached, return it */
    if (n < (idx_t)cache_size)
        return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Terms for the recurrence */
        seq_t term1 = ((seq_t)(3 * i) - (seq_t)1) * cache[i - 1];
        seq_t term2 = ((seq_t)(7 * i) - (seq_t)6) * cache[i - 2];
        seq_t term3 = ((seq_t)5 * (seq_t)(i - 1)) * cache[i - 3];

        /* a(i) = (term1 - term2 + term3) / i  (exact division) */
        cache[i] = (term1 - term2 + term3) / (seq_t)i;
    }
    cache_size = n + 1;

    return cache[n];
}
