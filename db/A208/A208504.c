/*
 * A208504  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A208504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-1) + 7*a(n-2) - 11*a(n-3) - 4*a(n-4) + 4*a(n-5)  (n>7)
 * Initial values (offset 1): 
 *   a(1)=19, a(2)=361, a(3)=456, a(4)=1406, a(5)=4104,
 *   a(6)=12654, a(7)=37430
 */

#include <stdlib.h>

/* Base terms a(1) … a(7) */
static const seq_t base_terms[7] = {19, 361, 456, 1406, 4104, 12654, 37430};

/* Cache for memoisation: 0‑based, cache[i] = a(i+1) */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n<1 */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_len = 7;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();       /* allocation failure */
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = base_terms[i];
    }

    /* Return already computed value if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* Expand cache to store up to n terms */
    idx_t new_len = n;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();             /* allocation failure */
    cache = tmp;
    cache_len = new_len;

    /* Compute missing terms using the recurrence */
    for (idx_t i = 7; i < n; ++i) {                     /* i is 0‑based: a(i+1) */
        cache[i] = 2 * cache[i - 1]
                 + 7 * cache[i - 2]
                 - 11 * cache[i - 3]
                 - 4 * cache[i - 4]
                 + 4 * cache[i - 5];
    }

    return cache[n - 1];
}
