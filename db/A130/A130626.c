/*
 * A130626  Second differences of A130624.
 * https://oeis.org/A130626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) Initial values: a(0)=3,
 *            a(1)=3, a(2)=4
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated capacity */
    static size_t len   = 0;      /* number of terms stored (next free index) */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed – unlikely in tests */

        cache[0] = (seq_t)3;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)4;
        len = 3;
    }

    /* If the requested term is already cached, return it */
    if (n < len) {
        return cache[n];
    }

    /* Grow capacity if needed */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) {
            newcap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* reallocation failed – unlikely */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute terms up to n using the recurrence */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        cache[i] = t1 * 3 - t2 * 3 + t3 * 2;
    }

    len = (size_t)n + 1;   /* we have computed up to index n */
    return cache[n];
}
