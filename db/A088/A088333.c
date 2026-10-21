/*
 * A088333  A version of Josephus problem: a(n) is the surviving integer under
 *          the following elimination process. Arrange 1,2,3,...,n in a
 *          circle, increasing clockwise. Starting with i=1, delete the
 *          integer 3 places clockwise from i. Repeat, counting 3 places from
 *          the next undeleted integer, until only one i...
 * https://oeis.org/A088333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(n)=((a(n-1)+3) mod n) + 1 for n>=2, where a(n) is the
 *            survivor in a Josephus problem with step size 3 (counting 3
 *            places clockwise from the current position)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: indices 1..max_cached contain previously computed values. */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    if (n < 1) {
        /* Undefined for n=0; return 0 as a safe fallback. */
        return (seq_t)0;
    }

    /* Allocate or enlarge cache as needed. */
    if (n > max_cached) {
        size_t new_size = (size_t)(n + 1);                /* +1 since we ignore index 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 (caller should handle). */
            return (seq_t)0;
        }
        cache = tmp;
        /* Compute new values from max_cached+1 up to n. */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
            } else {
                /* (cache[i-1] + 3) mod i + 1 */
                seq_t temp = cache[i - 1] + (seq_t)3;
                temp = temp % (seq_t)i;
                cache[i] = temp + (seq_t)1;
            }
        }
        max_cached = n;
    }
    return cache[n];
}
