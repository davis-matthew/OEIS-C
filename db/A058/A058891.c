/*
 * A058891  a(n) = 2^(2^(n-1) - 1).
 * https://oeis.org/A058891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2^(2^(n-1)-1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc, abort */

/* Return the n-th term of OEIS A058891.
   For n < 1 the return value is 0. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* cache of previously computed terms */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  size   = 0;

    size_t N = (size_t)n;

    /* extend cache if necessary */
    if (N > size) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap < N) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* memory allocation failure */

        cache = tmp;
        cap   = new_cap;

        /* compute terms from current size up to N-1 */
        for (size_t i = size; i < N; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;          /* a(1) = 1 */
            } else {
                seq_t prev = cache[i-1];
                cache[i]   = prev * prev;     /* prev^2   */
                cache[i]  *= (seq_t)2;        /* 2*prev^2 */
            }
        }
        size = N;
    }

    return cache[N-1];   /* a(n) */
}
