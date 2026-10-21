/*
 * A208690  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A208690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 10*a(n-2) + 2*a(n-3) - 10*a(n-4), * with a(1)=9,
 *            a(2)=81, a(3)=189, a(4)=927. * Computes the sequence for n>=1
 *            and memoises results
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Values below the offset are not required. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache: index 0 is unused. */
    static seq_t *cache    = NULL;
    static size_t  capacity = 0;
    static idx_t   max_computed = 0;

    /* Expand the cache if necessary. */
    size_t needed = (size_t)(n + 1);          /* index n is used */
    if (capacity < needed) {
        size_t new_cap = capacity ? capacity * 2 : 4;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp;
        if (cache == NULL)
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)             /* allocation failure – unlikely */
            return (seq_t)0;
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n.  Use the recurrence for i≥5. */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)9;
            } else if (i == 2) {
                cache[2] = (seq_t)81;
            } else if (i == 3) {
                cache[3] = (seq_t)189;
            } else if (i == 4) {
                cache[4] = (seq_t)927;
            } else {
                cache[i] = cache[i-1] + (seq_t)10 * cache[i-2]
                         + (seq_t)2  * cache[i-3] - (seq_t)10 * cache[i-4];
            }
        }
        max_computed = n;
    }

    return cache[n];
}
