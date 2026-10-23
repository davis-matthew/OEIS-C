/*
 * A143091  a(n) = a(floor(2n/3)) + a(floor(n/3)) starting a(0)=a(1)=1.
 * https://oeis.org/A143091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(floor(2*n/3)) + a(floor(n/3)), a(0) = a(1) = 1 (OEIS
 *            offset 0)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>


seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;      /* capacity of cache array */
    static idx_t  next   = 0;      /* smallest index not yet computed */

    /* extend cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? (cap << 1) : 8;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)1;         /* unlikely fallback */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values in ascending order */
    for (idx_t i = next; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = (seq_t)1;
        } else {
            idx_t i2 = i / 3;                 /* floor(n/3)     */
            idx_t i1 = i - ((i + 2) / 3);     /* floor(2*n/3)   */
            cache[i] = cache[i1] + cache[i2];
        }
    }
    next = n + 1;
    return cache[n];
}
