/*
 * A237250  Values of x in the solutions to x^2 - 4xy + y^2 + 11 = 0, where 0
 *          < x < y.
 * https://oeis.org/A237250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 67, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237250
   Recurrence: a(n) = 4*a(n-2) – a(n-4)   for n ≥ 5
   Initial values: a(1)=2, a(2)=3, a(3)=5, a(4)=10 */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Static cache to store computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated size of cache */
    static size_t len = 0;        /* number of terms currently in cache */

    /* Allocate or grow the cache if necessary */
    if (capacity < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failure */
        cache = new_cache;
        capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 1)          val = 2;
        else if (i == 2)     val = 3;
        else if (i == 3)     val = 5;
        else if (i == 4)     val = 10;
        else
            val = (seq_t)4 * cache[i - 3] - cache[i - 5];
        cache[i - 1] = val;
    }
    len = (size_t)n;

    return cache[n - 1];
}
