/*
 * A058797  a(n) = n*a(n-1) - a(n-2), with a(-1) = 0, a(0) = 1.
 * https://oeis.org/A058797
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) - a(n-2) Initial values: a(-1) = 0, a(0) = 1
 *            Recurrence holds for n ≥ 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 0 → a(-1), 1 → a(0), 2 → a(1), ...  */
    static seq_t *cache = NULL;   /* cached values          */
    static size_t cache_size = 0; /* number of elements     */

    if (n < -1)
        return (seq_t)0; /* undefined, return 0 */

    /* needed array size = n + 2 (index n+1) */
    size_t need = (size_t)n + 2;
    if (need == 0) /* n == -1 */
        need = 1;

    /* resize cache if necessary */
    if (cache_size < need) {
        cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */

        /* initialise newly allocated entries if needed */
        for (size_t i = cache_size; i < need; ++i) {
            if (i == 0)          /* a(-1) */
                cache[i] = (seq_t)0;
            else if (i == 1)     /* a(0) */
                cache[i] = (seq_t)1;
            else { /* recurrence for n = i-1 >= 1 */
                seq_t prev   = cache[i - 1]; /* a(i-1) */
                seq_t prev2  = cache[i - 2]; /* a(i-2) */
                cache[i] = ((seq_t)(i - 1)) * prev - prev2;
            }
        }
        cache_size = need;
    }

    return cache[(size_t)n + 1];
}
