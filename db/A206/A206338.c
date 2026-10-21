/*
 * A206338  Number of (n+1) X 4 0..3 arrays with every 2 X 2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A206338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 30, u64 n <= 70, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3)  for n > 12
   (first 12 terms are the known initial values of the sequence) */

#include <stdlib.h>

/* static cache: cache[i] holds a(i+1), i.e. a(1) is at cache[0] */
static seq_t *cache = NULL;
static idx_t cached_n = 0;          /* how many terms are currently cached */

/* the 12 seed values of the sequence */
static const seq_t init[12] = {
    (seq_t)455,  (seq_t)232,  (seq_t)1340, (seq_t)682,
    (seq_t)4007, (seq_t)2030, (seq_t)12005,(seq_t)6074,
    (seq_t)35999,(seq_t)18206,(seq_t)107983,(seq_t)54606
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* sequence has offset 1 */

    /* grow the cache if we have not computed up to n yet */
    if (cached_n < n) {
        size_t newsize = (size_t)n;              /* size_t is the type for malloc/realloc size */
        seq_t *new_cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!new_cache) return 0;                /* allocation failed – return 0 as error value */

        cache = new_cache;

        /* compute the missing terms */
        for (idx_t i = cached_n; i < n; ++i) {
            if (i < 12) {
                cache[i] = init[i];
            } else {
                cache[i] = cache[i-1] +
                           (cache[i-2] * (seq_t)3) -
                           (cache[i-3] * (seq_t)3);
            }
        }
        cached_n = n;
    }

    return cache[n-1];
}
