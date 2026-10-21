/*
 * A097228  Numbers n such that the product of digits of n equals the
 *          concatenation of pi(d)'s where d runs through the digits of n.
 * https://oeis.org/A097228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 11*a(n-4) - 10*a(n-8) for n > 10 with initial values:
 *            a(1)=27, a(2)=38, a(3)=127, a(4)=138, a(5)=289, a(6)=298,
 *            a(7)=1127, a(8)=1138, a(9)=1289, a(10)=1298 (indices start at 1)
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 34, u64 n <= 74, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of A097228 (1‑based index).  */
seq_t a(idx_t n) {
    /* static cache to avoid recomputing already known terms  */
    static idx_t cache_size = 0;     /* number of allocated elements (including 0‑th unused slot) */
    static seq_t *cache = NULL;      /* cache[i] holds a(i), i>=1 */

    /* initial terms (a(1) … a(10)) */
    static const seq_t init[11] = {
        0,          /* unused 0th index */
        (seq_t)27,  (seq_t)38,  (seq_t)127, (seq_t)138,
        (seq_t)289, (seq_t)298, (seq_t)1127, (seq_t)1138,
        (seq_t)1289, (seq_t)1298
    };

    /* allocate and fill initial cache if needed */
    if (cache_size == 0) {
        cache_size = 11;                      /* indices 1..10 */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        for (idx_t i = 1; i < cache_size; ++i)
            cache[i] = init[i];
    }

    /* extend cache up to requested n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;                 /* need indices up to n */
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (!new_cache) { /* allocation failed: fallback to initial terms up to available size */
            return init[n < 11 ? n : 10];
        }
        cache = new_cache;
        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = init[i];
            } else {
                /* recurrence: a(i) = 11*a(i-4) - 10*a(i-8)  */
                seq_t t1 = (seq_t)11 * cache[i - 4];
                seq_t t2 = (seq_t)10 * cache[i - 8];
                cache[i] = t1 - t2;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
