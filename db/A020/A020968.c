/*
 * A020968  Expansion of 1/((1-7*x)*(1-8*x)*(1-11*x)).
 * https://oeis.org/A020968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A020968: expansion of 1/((1-7*x)*(1-8*x)*(1-11*x))
   Formula: a(n) = 26*a(n-1) - 221*a(n-2) + 616*a(n-3)   for n>=3
   Initial values: a(0)=1, a(1)=26, a(2)=455
   Computes terms on demand using memoisation and an iterative recurrence. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;     /* allocated size of cache (number of slots) */
    static idx_t last = 0;         /* highest index that has been computed */

    /* Initialise cache with first three terms */
    if (cache == NULL) {
        capacity = 3;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;     /* out‑of‑memory: return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)455;
        last = 2;
    }

    /* Grow the cache array if necessary */
    if (n >= capacity) {
        while (capacity <= n) {
            capacity <<= 1;                    /* double until large enough */
        }
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;     /* out‑of‑memory */
        cache = tmp;
    }

    /* Compute missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = (seq_t)26 * cache[i - 1]
                     - (seq_t)221 * cache[i - 2]
                     + (seq_t)616 * cache[i - 3];
        }
        last = n;
    }

    return cache[n];
}
