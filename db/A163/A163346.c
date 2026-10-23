/*
 * A163346  a(n) = 10*a(n-1) - 23*a(n-2) for n > 1; a(0) = 1, a(1) = 7.
 * https://oeis.org/A163346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 23*a(n-2), a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;

    /* Initialise cache with first two terms if necessary */
    if (!cache) {
        cap = 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        sz = 2;
    }

    /* Grow cache capacity if needed */
    if (n + 1 > (idx_t)cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory – unlikely */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    if ((size_t)n + 1 > sz) {
        for (size_t i = sz; i <= (size_t)n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            cache[i] = (seq_t)10 * a1 - (seq_t)23 * a2;
        }
        sz = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
