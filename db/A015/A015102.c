/*
 * A015102  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=-6.
 * https://oeis.org/A015102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 ; a(n+1) = Σ_{i=0..n} (-6)^i * a(i) * a(n-i) (OEIS
 *            A015102)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache_seq = NULL;   /* a(n) values */
static seq_t *cache_pow = NULL;   /* (-6)^i values */
static size_t  cache_size = 0;    /* current number of stored terms */

/* Ensure that a(n) is computed and cached for all n ≤ upper */
static void ensure_cache(idx_t upper)
{
    size_t needed = (size_t)upper + 1;
    if (needed <= cache_size) return;          /* already available */

    /* Grow the arrays */
    #if defined(__cplusplus)
        seq_t *tmp_seq = static_cast<seq_t*>(realloc(cache_seq, needed * sizeof(seq_t)));
        seq_t *tmp_pow = static_cast<seq_t*>(realloc(cache_pow, needed * sizeof(seq_t)));
    #else
        seq_t *tmp_seq = (seq_t*)realloc(cache_seq, needed * sizeof(seq_t));
        seq_t *tmp_pow = (seq_t*)realloc(cache_pow, needed * sizeof(seq_t));
    #endif
    if (!tmp_seq || !tmp_pow) abort();          /* allocation failed */
    cache_seq = tmp_seq;
    cache_pow = tmp_pow;

    /* If this is the first allocation, initialise the base case */
    if (cache_size == 0) {
        cache_seq[0] = (seq_t)1;
        cache_pow[0] = (seq_t)1;
        cache_size = 1;                         /* now we have a(0) and (-6)^0 */
    }

    /* Compute new powers of q = -6 */
    for (size_t i = cache_size; i < needed; ++i)
        cache_pow[i] = cache_pow[i - 1] * (seq_t)(-6);

    /* Compute new terms a(k), for k = cache_size .. needed‑1 */
    for (size_t k = cache_size; k < needed; ++k) {
        seq_t sum = (seq_t)0;
        /* a(k) = Σ_{i=0..k-1} (-6)^i * a(i) * a(k-1-i) */
        for (size_t i = 0; i < k; ++i)
            sum += cache_pow[i] * cache_seq[i] * cache_seq[k - 1 - i];
        cache_seq[k] = sum;
    }

    cache_size = needed;
}

/* Public function: returns a(n) for any n ≥ 0 */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache_seq[(size_t)n];
}
