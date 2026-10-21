/*
 * A078446  a(1)=a(2)=1; a(n)=a(n-2)/2 if a(n-2) is even, a(n)=a(n-1)+a(n-2)
 *          otherwise.
 * https://oeis.org/A078446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 257, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = a(2) = 1;
 *        for n ≥ 3   a(n) = { a(n-2)/2   if a(n-2) is even,
 *                            a(n-1)+a(n-2) otherwise }.
 * This implementation memoises the sequence values in a static array
 * whose size grows as needed when higher indices are queried.  The cache
 * starts out empty and grows by doubling its capacity; a tiny dummy entry
 * at index 0 is kept only as a placeholder.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based cache: index 0 is unused but maintained for simplicity */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size (number of slots) */
    static idx_t cache_max = 0;   /* highest index that has a value stored */

    if (n == 0) return 0;         /* undefined – return 0 */

    /* Resize cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n) new_cap <<= 1;        /* at least n+1 slots */

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);                  /* allocation failure */

        /* Initialise new slots to zero */
        for (idx_t i = 0; i < new_cap; ++i) new_cache[i] = 0;

        /* Copy existing values */
        if (cache) {
            for (idx_t i = 0; i <= cache_max; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1 || i == 2) {
            val = 1;
        } else {
            seq_t a_prev = cache[i - 1];
            seq_t a_prevprev = cache[i - 2];
            if ((a_prevprev & 1) == 0)  /* a(n-2) even */
                val = a_prevprev >> 1;  /* divide by 2 */
            else
                val = a_prev + a_prevprev;
        }
        cache[i] = val;
    }
    cache_max = n;
    return cache[n];
}
