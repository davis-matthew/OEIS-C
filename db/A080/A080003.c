/*
 * A080003  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=3, I={1,2}.
 * https://oeis.org/A080003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-3)+2*a(n-4)+2*a(n-5)+a(n-7)-a(n-8)-a(n-10)
   Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=4, a(5)=8, a(6)=13,
   a(7)=22, a(8)=40, a(9)=77, a(10)=140 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Historical values for n < 11 */
    static const seq_t init[11] = {1, 1, 1, 1, 4, 8, 13, 22, 40, 77, 140};

    /* Static cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* memory allocated (in elements) */
    static size_t computed_len = 0; /* up to which index cache has been filled */

    /* Ensure there is enough memory to address index n */
    if (cache_size <= (size_t)n) {
        size_t new_size = cache_size == 0 ? 16 : cache_size * 2;
        if (new_size <= (size_t)n) new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failure */
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute missing values up to n */
    if (computed_len <= (size_t)n) {
        size_t start = computed_len;
        for (size_t i = start; i <= (size_t)n; ++i) {
            if (i <= 10) {
                cache[i] = init[i];
            } else {
                seq_t val = cache[i-1] + cache[i-3] + cache[i-7];
                val += cache[i-4];
                val += cache[i-4];
                val += cache[i-5];
                val += cache[i-5];
                val -= cache[i-8];
                val -= cache[i-10];
                cache[i] = val;
            }
        }
        computed_len = (size_t)n + 1;
    }

    return cache[n];
}
