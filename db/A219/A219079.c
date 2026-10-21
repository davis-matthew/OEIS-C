/*
 * A219079  Number of 2 X n binary arrays with every 0 a horizontal, diagonal
 *          or antidiagonal neighbor to some 1.
 * https://oeis.org/A219079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 7*a(n-3) + 6*a(n-4), with initial
 *            values a(1)=1, a(2)=11, a(3)=47, a(4)=165
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of previously computed values: a(1) at index 0 */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated size (in elements)   */
    static idx_t last = 0;      /* largest n for which a(n) was computed   */

    if (n == 0) return 0; /* undefined offset, return 0 */

    /* Allocate or enlarge the cache to hold up to n entries */
    if (cache == NULL || n > (idx_t)(cap - 1)) {
        size_t new_cap = (size_t)(n + 4);      /* ensure space for indices 0..n-1 */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (cache == NULL) return 0;      /* allocation failure */
            /* initialise the first values */
            cache[0] = 1;
            cache[1] = 11;
            cache[2] = 47;
            cache[3] = 165;
            last = 4;
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;        /* allocation failure */
            cache = tmp;
            /* previously computed entries are preserved by realloc */
        }
        cap = new_cap;
    }

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = (last > 4 ? last : 4) + 1; i <= n; ++i) {
        seq_t v1 = cache[(size_t)(i - 2)]; /* a(i-1) */
        seq_t v2 = cache[(size_t)(i - 3)]; /* a(i-2) */
        seq_t v3 = cache[(size_t)(i - 4)]; /* a(i-3) */
        seq_t v4 = cache[(size_t)(i - 5)]; /* a(i-4) */
        cache[(size_t)(i - 1)] = (seq_t)3 * v1 + v2 + (seq_t)7 * v3 + (seq_t)6 * v4;
    }
    last = n;

    return cache[(size_t)(n - 1)];
}
