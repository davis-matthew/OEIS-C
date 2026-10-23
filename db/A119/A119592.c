/*
 * A119592  a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=7, a(5)=10; a(n) =
 *          floor(a(n-1) + 1 + (a(n-2) + 1)/6) for n>=6.
 * https://oeis.org/A119592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=7, a(5)=10; for n>=6:
 *            a(n)=a(n-1)+1+(a(n-2)+1)/6 (floor is implicit integer division)
 * Range:     u8 n <= 26, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache that grows as needed. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t computed = 0;  /* number of terms already computed */

    /* Initialise the cache on the first call.  Allocate space for at least
       the first six terms, because they are needed for all later values.  */
    if (!cache) {
        size_t init = 6;
        if ((size_t)n + 1 > init) init = (size_t)n + 1;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) return 0;  /* out‑of‑memory: caller will not use the value */
        capacity = init;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)7;
        cache[5] = (seq_t)10;
        computed = 6;
    }

    /* Grow the array if the requested index exceeds the current capacity. */
    if ((size_t)n + 1 > capacity) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed */
        cache = tmp;
        capacity = newcap;
    }

    /* Return cached value if already computed. */
    if (n < computed) return cache[n];

    /* Compute all missing terms up to n.  The recurrence uses only integer
       operations on seq_t, so it stays within the allowed expression set. */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = prev1 + (seq_t)1 + (prev2 + (seq_t)1) / (seq_t)6;
    }
    computed = n + 1;

    return cache[n];
}
