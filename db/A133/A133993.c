/*
 * A133993  a(n) = a(n-1) + 3*a(n-2) - a(n-3) - 2*a(n-4), n > 3.
 * https://oeis.org/A133993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3) - 2*a(n-4), a(0) = 1, a(1) =
 *            2, a(2) = 3, a(3) = 4
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of already computed results */
    static seq_t *cache = NULL;      /* dynamic array */
    static size_t allocated = 0;     /* number of slots allocated */
    static size_t computed = 0;      /* largest index computed + 1 */

    /* Base cases for n < 4 */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)2;
            case 2: return (seq_t)3;
            case 3: return (seq_t)4;
        }
    }

    /* Ensure enough space for the requested index */
    if ((size_t)n + 1 > allocated) {
        size_t new_alloc = ((size_t)n + 1) * 2;   /* grow by factor 2 */
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – abort */
            abort();
        }
        cache = tmp;
        allocated = new_alloc;
    }

    /* Initialize base values on first use */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        computed = 4;
    }

    /* Compute missing entries up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t temp = cache[i-1] + (seq_t)3 * cache[i-2];
        temp -= cache[i-3];
        temp -= (seq_t)2 * cache[i-4];
        cache[i] = temp;
    }
    computed = n + 1;

    return cache[(size_t)n];
}
