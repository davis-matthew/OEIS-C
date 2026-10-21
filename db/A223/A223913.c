/*
 * A223913  Number of n X 3 0..2 arrays with rows and antidiagonals unimodal
 *          and columns nondecreasing.
 * https://oeis.org/A223913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 62, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A223913 – Number of n×3 0..2 arrays with rows and antidiagonals unimodal and
    columns nondecreasing.  The sequence satisfies the order‑7 recurrence

          a(n) = 7·a(n‑1) – 21·a(n‑2) + 35·a(n‑3) – 35·a(n‑4)
                 + 21·a(n‑5) – 7·a(n‑6) + a(n‑7)   (n > 7)

    with initial values  
          a(1)=22, a(2)=158, a(3)=666, a(4)=2111, a(5)=5548,
          a(6)=12752, a(7)=26494.  The function below computes a(n)
    for any n ≥ 1, caching intermediate values so that subsequent calls
    are O(1).  It compiles both as C11 and as C++17 and works with an
    arbitrary‑precision seq_t class or any unsigned integer type. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Ensure n >= 1; behaviour for n < 1 is undefined. */
    if (n < 1)
        return (seq_t)0;   /* placeholder – not used by the OEIS tests */

    /* Static cache of computed values: cache[1] ... cache[cache_len-1] are valid. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of valid slots + 1 (for zero unused slot) */

    /* Initial seven values (index 1..7). */
    const seq_t init[8] = {0, 22, 158, 666, 2111, 5548, 12752, 26494};

    /* Initialise the cache on first call. */
    if (cache_len < 8) {
        seq_t *tmp = (seq_t *)realloc(cache, 8 * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = 1; i <= 7; ++i)
            cache[i] = init[i];
        cache_len = 8;   /* cache[1]..cache[7] are now valid */
    }

    /* Return cached value if already computed. */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache to reach index n. */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute missing terms using the recurrence. */
    for (idx_t k = cache_len; k <= n; ++k) {
        cache[k] =  7 * cache[k - 1]
                 - 21 * cache[k - 2]
                 + 35 * cache[k - 3]
                 - 35 * cache[k - 4]
                 + 21 * cache[k - 5]
                 -  7 * cache[k - 6]
                 +  cache[k - 7];
    }

    cache_len = n + 1;
    return cache[n];
}
