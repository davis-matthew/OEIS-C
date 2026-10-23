/*
 * A186371  Number of up-down runs in all permutations of {1,2,...,n}.
 * https://oeis.org/A186371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * (4n + 1) / 6 for n >= 2 a(0) = 0, a(1) = 1 (n! is
 *            computed iteratively and cached)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static cache for factorial values */
static seq_t *fact_cache = NULL;   /* fact_cache[i] = i!  (fact_cache[0] = 1) */
static size_t fact_cache_len = 0;  /* number of elements stored */

/* Ensure the factorial cache contains at least 0! … n! */
static void ensure_fact_cache(idx_t n)
{
    if (fact_cache == NULL) {
        fact_cache = (seq_t *)malloc(sizeof(seq_t));
        if (fact_cache == NULL) return;          /* allocation error – fail silently */
        fact_cache[0] = (seq_t)1;
        fact_cache_len = 1;
    }

    if ((size_t)n + 1 <= fact_cache_len) return; /* already present */

    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(fact_cache, new_len * sizeof(seq_t));
    if (new_cache == NULL) return;   /* allocation error – fail silently */
    fact_cache = new_cache;

    seq_t cur = fact_cache[fact_cache_len - 1];   /* (len-1)!  */
    for (size_t i = fact_cache_len; i < new_len; ++i) {
        cur = cur * (seq_t)i;
        fact_cache[i] = cur;
    }
    fact_cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    ensure_fact_cache(n);
    if (fact_cache == NULL) return (seq_t)0;   /* allocation failed */

    seq_t fac = fact_cache[(size_t)n];
    seq_t mult = fac * ((seq_t)(4ULL * n + 1));
    return mult / (seq_t)6;
}
