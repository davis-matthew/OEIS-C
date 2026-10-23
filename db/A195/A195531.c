/*
 * A195531  Hypotenuses of Pythagorean triples in A195499 and A195503.
 * https://oeis.org/A195531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Conjecture: a(n) = 3*a(n-1) + 3*a(n-2) - a(n-3)
 * Initial values: a(1) = 5, a(2) = 17, a(3) = 65
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache to store previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of computed elements */

    if (n == 0)   /* offset is 1; behaviour for n==0 is undefined */
        return 0;

    /* First allocation or grow cache if needed */
    if (cache == NULL) {
        /* Ensure space for at least the first three terms */
        idx_t capacity = (n > 3) ? n : 3;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = 5;                /* a(1) */
        cache[1] = 17;               /* a(2) */
        cache[2] = 65;               /* a(3) */
        cache_len = capacity;
    } else if (cache_len < n) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        /* If we never had all three base terms (possible on first resize)
           initialise them now.  They are already correct for the smallest
           indices but this is a cheap double‑write guard. */
        if (cache_len < 3) {
            if (cache_len < 1) cache[0] = 5;
            if (cache_len < 2) cache[1] = 17;
            if (cache_len < 3) cache[2] = 65;
        }
        cache_len = new_len;
    }

    /* Compute any missing terms using the recurrence */
    for (idx_t i = 3; i < cache_len; ++i) {
        cache[i] = 3 * cache[i - 1] + 3 * cache[i - 2] - cache[i - 3];
    }

    return cache[n - 1];
}
