/*
 * A183712  1/20 of the number of (n+1) X 3 0..4 arrays with every 2 X 2
 *          subblock strictly increasing clockwise or counterclockwise with
 *          one decrease.
 * https://oeis.org/A183712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - a(n-3); a(1) = 5, a(2) = 17, a(3) =
 *            54
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;         /* cache of computed values */
    static size_t cap   = 0;            /* allocated capacity (indices 0..cap) */
    static idx_t  last  = 0;            /* highest index already computed */

    /* trivial case */
    if (n == 0) return (seq_t)0;

    /* enlarge cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap == 0 ? 4 : cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failed – abort */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise base cases once */
    if (last == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)54;
        last    = 3;
    }

    if (n <= last) return cache[n];

    /* compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i-1] + cache[i-2] - cache[i-3];
    }
    last = n;
    return cache[n];
}
