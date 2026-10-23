/*
 * A129288  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2 + (x + 41)^2 = y^2.
 * https://oeis.org/A129288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 33, u64 n <= 70, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A129288:
 * a(1)=0, a(2)=36, a(3)=39, a(4)=123, a(5)=319, a(6)=336;
 * for n>6:  a(n) = 6*a(n-3) - a(n-6) + 82
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* a(1) is the first term; any n < 1 is undefined, return 0 */
    if (n < 1)
        return 0;

    /* static cache: cache[i] holds a(i+1)  (zero‑based) */
    static seq_t *cache = NULL;
    static idx_t size   = 0;   /* number of terms currently cached */

    /* First call: allocate and initialise the first six terms */
    if (!cache) {
        size = 6;
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (!cache)
            return 0;   /* allocation failed */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)36;
        cache[2] = (seq_t)39;
        cache[3] = (seq_t)123;
        cache[4] = (seq_t)319;
        cache[5] = (seq_t)336;
    }

    /* If the desired term is already computed, return it */
    if (n <= size)
        return cache[n - 1];

    /* Extend the cache to include a(n) */
    idx_t old_size = size;
    idx_t new_size = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        return 0;   /* reallocation failed */

    cache = tmp;
    size  = new_size;

    /* Compute new terms using the recurrence */
    for (idx_t i = old_size; i < size; ++i) {
        /* i is zero‑based index of a(i+1); i ≥ 6 guarantees i-3 and i-6 exist. */
        cache[i] = (seq_t)6 * cache[i - 3] - cache[i - 6] + (seq_t)82;
    }

    return cache[n - 1];
}
