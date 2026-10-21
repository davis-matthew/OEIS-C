/*
 * A224869  a(n) = n*( a(n-1)+1 ), initialized by a(1) = -1.
 * https://oeis.org/A224869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A224869 :  a(n) = n*(a(n-1)+1) ,  a(1) = -1  */
#include <stdlib.h>

/* Static cache for memoised values: cache[1] == a(1). */
static seq_t *cache = NULL;      /* pointer to cache array */
static size_t cache_cap = 0;     /* allocated length of cache */
static idx_t  computed_max = 0;  /* highest index realised so far */

/* Return the n-th term of sequence A224869 (n ≥ 1).  The function
   may be called in arbitrary order; the values are memoised in a
   static array. */
seq_t a(idx_t n)
{
    if (n < 1)          /* out of defined range – return 0 */
        return (seq_t)0;

    /* Initialise the base case (n == 1).  Allocate an array of
       at least two elements so that cache[1] is usable. */
    if (computed_max < 1) {
        if (!cache || cache_cap < 2) {
            cache = (seq_t *)calloc(2, sizeof(seq_t));
            if (!cache) return (seq_t)0;   /* allocation failure */
            cache_cap = 2;
        }
        cache[1] = (seq_t)-1;   /* a(1) = -1 */
        computed_max = 1;
    }

    /* Expand the cache if it is too small for the requested index. */
    if ((size_t)n + 1 > cache_cap) {
        void *tmp = realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = (seq_t *)tmp;
        cache_cap = (size_t)n + 1;
    }

    /* Compute missing terms up to n.  The loop is safe even if the
       requests are in decreasing order because computed_max tracks
       the highest index that has already been calculated. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = (seq_t)i * (prev + (seq_t)1);
    }
    computed_max = n;

    return cache[n];
}
