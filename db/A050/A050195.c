/*
 * A050195  a(n)=a(n-1)+a(n-2)-d, where d=a(n/5) if 5 divides n, else d=0; 2
 *          initial terms.
 * https://oeis.org/A050195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-d, d = a(n/5) if 5 divides n, else d = 0;
 *            Initial values: a(1) = 1, a(2) = 1
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;   /* array of a[0..cap-1] */
    static idx_t cap   = 0;       /* current capacity */
    static idx_t last  = 0;       /* highest n computed (0 means none) */

    /* Expand cache if necessary.  +1 because we use 1‑based indices. */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap << 1 : 4;
        if (new_cap <= n)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE); /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms up to n if we haven't already. */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1 || i == 2) {
            cache[i] = (seq_t)1;
        } else {
            seq_t d = 0;
            if (i % 5 == 0)
                d = cache[i / 5];
            cache[i] = cache[i - 1] + cache[i - 2] - d;
        }
    }
    last = n;

    return cache[n];
}
