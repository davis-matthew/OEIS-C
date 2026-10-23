/*
 * A024852  Duplicate of A023856.
 * https://oeis.org/A024852
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 21, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A024852 (duplicate of A023856)
 * Recurrence:
 *   a(2) = 2
 *   for n ≥ 3
 *     if n is odd  → a(n) = a(n-1) + T((n-1)/2)
 *     if n is even → a(n) = a(n-1) + (n/2)*(3*n/2 + 1)/2
 *   where T(k)=k*(k+1)/2 is the k‑th triangular number.
 * The definition reproduces the first terms:
 *   a(2)=2, a(3)=3, a(4)=10, a(5)=13, a(6)=28, a(7)=34, a(8)=60, …
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset (2).  Undefined behaviour is
     * acceptable for the problem, but returning 0 is harmless. */
    if (n < 2) return (seq_t)0;

    /* Static cache for memoization.  cache[i] holds a(i).  The cache
     * is grown as needed so repeated calls are O(1) after the first. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of allocated elements */
    static idx_t  cache_max = 1;   /* highest n that has been computed */

    /* Ensure capacity for indices up to n.  Use realloc to grow the
     * buffer.  The buffer always contains indices 0..(cache_cap-1). */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        /* double the size when possible for fewer reallocations */
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n if not already done. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t diff;

        if (i & 1U) { /* odd n */
            idx_t k = (i - 1) / 2;            /* k ≥ 1 for i ≥ 3 */
            diff = ((seq_t)k * (seq_t)(k + 1)) / 2;  /* T(k) */
        } else { /* even n */
            idx_t m = i / 2;                /* m ≥ 2 for i ≥ 4 */
            diff = ((seq_t)m * (seq_t)(3 * m + 1)) / 2;
        }

        cache[i] = prev + diff;
    }
    cache_max = n;

    return cache[n];
}
