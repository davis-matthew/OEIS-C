/*
 * A015473  q-Fibonacci numbers for q=2, scale a(n-1).
 * https://oeis.org/A015473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n)=2^(n-1)*a(n-1)+a(n-2)  (n≥2) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array of computed values   */
    static idx_t  computed = 0;     /* number of values stored    */
    static size_t capacity = 0;     /* allocated capacity of cache*/
    static seq_t pow2 = 1;          /* 2^(i-1) for next i          */

    /* Initialise cache with the first two terms */
    if (cache == NULL) {
        capacity = 2;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed = 2;
    }

    /* Expand cache if needed */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = capacity;
        while (new_cap < (size_t)(n + 1)) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        capacity = new_cap;
    }

    /* Return cached value if already computed */
    if (n + 1 <= computed) return cache[n];

    /* Compute terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        /* pow2 now holds 2^(i-1) */
        pow2 <<= 1;                      /* shift to next power of two */
        cache[i] = pow2 * cache[i - 1] + cache[i - 2];
    }
    computed = n + 1;
    return cache[n];
}
