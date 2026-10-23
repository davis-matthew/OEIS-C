/*
 * A158783  a(n) = Hermite(n, 24).
 * https://oeis.org/A158783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 48*a(n-1) - 2*(n-1)*a(n-2) Initial values: a(0) = 1, a(1)
 *            = 48 Recurrence: a(n) = 48*a(n-1) - 2*(n-1)*a(n-2) (n >= 2)
 *            Offset: 0 (a(0) is the first term) This implementation memoises
 *            the sequence in a dynamic array so that any call order is
 *            handled efficiently. It works for 8/16/32/64/128-b...
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;            /* dynamic cache for sequence values   */
static size_t  cap   = 0;              /* currently allocated size of cache   */
static idx_t   cached_max = 0;         /* largest index that has been computed */

/* Grow the cache to be able to store indices up to n, inclusive.  */
static void grow_cache(idx_t n)
{
    size_t need = (size_t)(n + 1);     /* indices 0..n inclusive             */
    if (need <= cap) return;

    size_t new_cap = cap ? cap * 2 : 1;
    if (new_cap < need) new_cap = need;

    /* realloc returns void*; cast explicitly for C++11 compliance. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failure                */
    cache = tmp;
    cap   = new_cap;
}

/* Public function required by the evaluation harness. */
seq_t a(idx_t n)
{
    /* Lazy initialisation of base cases.  The first time this function
       is called we allocate space for indices 0 and 1 and store
       a(0) = 1, a(1) = 48. */
    if (cache == NULL) {
        grow_cache(1);                /* needs indices 0 and 1             */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)48;
        cached_max = 1;
    }

    /* If the caller requests a value beyond the current cache, grow it. */
    if (n >= cap) {
        grow_cache(n);
    }

    /* Return immediately if the value is already known. */
    if (n <= cached_max) {
        return cache[n];
    }

    /* Compute the missing terms using the recurrence.  All products
       and additions are performed in seq_t so that the final result
       is exact for the chosen seq_t type. */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)48 * cache[i - 1];
        seq_t term2 = ((seq_t)2 * (seq_t)(i - 1)) * cache[i - 2];
        cache[i] = term1 - term2;
    }

    cached_max = n;
    return cache[n];
}
