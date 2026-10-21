/*
 * A080047  Operation count to create all permutations of n distinct elements
 *          using Algorithm L (lexicographic permutation generation) from
 *          Knuth's The Art of Computer Programming, Vol. 4, chapter 7.2.1.2.
 *          Sequence gives number of times l has to be repeatedly decreased in
 *          step L3.
 * https://oeis.org/A080047
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * a(n-1) + (n-1)*(n-2)/2 for n >= 3 a(2) = 0, a(3) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the given recurrence; the first two values are
   initialised explicitly.  The array is grown with realloc
   and indexed directly by n (indices 0 and 1 are unused). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is valid for n >= 2.  For n < 2 the value is undefined
       for the purpose of the sequence; we return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Static cache of previously computed values.  The array is
       indexed directly by n, so cache[i] holds a(i).  We keep
       the largest n for which the cache is valid in max_cached. */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    /* Grow the cache if necessary.  realloc returns void*, so
       we cast it explicitly to seq_t*.  The new size is n+1 to
       allow direct indexing up to n. */
    if (n > max_cached) {
        seq_t *new_cache =
            (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;           /* allocation failure → 0 */

        cache = new_cache;

        /* Initialise every new element from the last cached index
           up to n.  Indices 0 and 1 are not used; we start
           from max_cached+1 but not smaller than 2. */
        idx_t start = max_cached + 1;
        if (start < 2)
            start = 2;

        for (idx_t i = start; i <= n; ++i) {
            if (i == 2) {
                cache[i] = (seq_t)0;
            } else if (i == 3) {
                cache[i] = (seq_t)1;
            } else {
                /* a(i) = i * a(i-1) + (i-1)*(i-2)/2  */
                cache[i] = (seq_t)i * cache[i - 1] +
                           ((seq_t)(i - 1) * (i - 2) / 2);
            }
        }
        max_cached = n;
    }
    return cache[n];
}
