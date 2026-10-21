/*
 * A226275  Number of new rationals produced at the n-th iteration by applying
 *          the map t -> {t+1, -1/t} to nonzero terms, starting with S[0] =
 *          {1}.
 * https://oeis.org/A226275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 57, u64 n <= 115,
 *            u128 n <= 232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A226275
   Recurrence:
     a(0) = 1, a(1) = 2, a(2) = 3,
     a(3) = 3, a(4) = 5, a(5) = 7,
     a(n) = a(n-1) + a(n-3)   for n >= 6
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0; /* allocated capacity in elements           */
    static size_t len   = 0; /* number of terms stored (next free index) */

    /* ensure we have enough capacity for indices 0 … n */
    size_t needed = (size_t)n + 1;
    if (needed < 6)
        needed = 6;                         /* base needs 6 elements   */
    if (cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure      */
        cache = tmp;
        cap   = needed;
    }

    /* initialise base terms on first use */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)7;
        len = 6;
    }

    /* if the requested index is already cached, return it */
    if (n < len)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 3];
    }
    len = (size_t)n + 1;
    return cache[n];
}
