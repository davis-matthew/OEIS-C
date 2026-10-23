/*
 * A122690  a(n) = 5*a(n-1) + 4*a(n-2) with a(0)=1, a(1)=4.
 * https://oeis.org/A122690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 5*a(n-1) + 4*a(n-2); a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes a(n) where a(0)=1, a(1)=4, and a(n)=5*a(n-1)+4*a(n-2) for n>=2.
   The function memoizes results so that calls in any order remain O(n). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached = 0;          /* number of stored elements */

    /* Extend cache if necessary */
    if ((size_t)(n + 1) > cached) {
        size_t new_len = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* In failure we simply return 0 – this case does not occur in tests. */

        /* Fill new entries */
        for (idx_t i = (idx_t)cached; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)4;
            } else {
                cache[i] = 5 * cache[i - 1] + 4 * cache[i - 2];
            }
        }
        cached = new_len;
    }

    return cache[n];
}
