/*
 * A032924  Numbers whose ternary expansion contains no 0.
 * https://oeis.org/A032924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Numbers whose ternary expansion contains no 0. Recurrence: a(1)
 *            = 1; a(2*n) = a(2*n-1) + 1; a(2*n+1) = 3*a(n) + 1; (n >= 1)
 * Range:     u8 n <= 62, u16 n <= 2046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is stored lazily in a static 1‑indexed array.  */
    static seq_t *cache = NULL;    /* cache[i] holds a(i) for i >= 1    */
    static idx_t  cap   = 0;       /* largest index currently in cache */

    if (n == 0) return 0;          /* not defined for n=0, harmless guard */

    if (n > cap) {
        /* Grow the cache to hold indices up to n.  */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – abort in the usual C way.  */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;

        /* Compute new entries in increasing order.  */
        for (idx_t i = cap + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
            } else if ((i & 1) == 0) {          /* i even: a(i) = a(i-1) + 1 */
                cache[i] = cache[i - 1] + (seq_t)1;
            } else {                           /* i odd : a(i) = 3*a((i-1)/2) + 1 */
                cache[i] = (seq_t)3 * cache[(i - 1) >> 1] + (seq_t)1;
            }
        }
        cap = n;
    }
    return cache[n];
}
