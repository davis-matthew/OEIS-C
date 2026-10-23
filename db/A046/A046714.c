/*
 * A046714  Convolution of A000108 (Catalan) with A000351 (powers of 5).
 * https://oeis.org/A046714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + Catalan(n); a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values for a(n) */
static seq_t *a_cache = NULL;   /* array of a values, 0..cached_to */
static idx_t    cached_to = 0;  /* highest index that has been computed */
static seq_t    cat_last   = 1;  /* Catalan(cached_to), used to compute next Catalan */

seq_t a(idx_t n)
{
    /* Initialise cache for n = 0 on first call */
    if (a_cache == NULL) {
        a_cache   = (seq_t *)malloc(sizeof(seq_t));  /* capacity for index 0 */
        if (a_cache == NULL) abort();                /* allocation failure */
        a_cache[0] = (seq_t)1;                       /* a(0) = 1 */
        cached_to  = 0;
        cat_last   = (seq_t)1;                       /* Catalan(0) = 1 */
    }

    /* If already computed, simply return the cached value */
    if (n <= cached_to)
        return a_cache[n];

    /* Extend the cache to hold values up to n */
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) abort();
    a_cache = new_cache;

    /* Compute further terms using the recurrence */
    for (idx_t idx = cached_to + 1; idx <= n; ++idx) {
        /* Update Catalan number: C(idx) = C(idx-1) * (4*idx - 2) / (idx + 1) */
        cat_last = cat_last * (((seq_t)idx * 4u) - 2u);
        cat_last = cat_last / ((seq_t)(idx + 1u));

        /* a(idx) = 5 * a(idx-1) + Catalan(idx) */
        a_cache[idx] = a_cache[idx - 1] * (seq_t)5 + cat_last;
    }

    cached_to = n;
    return a_cache[n];
}
