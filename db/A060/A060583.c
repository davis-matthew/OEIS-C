/*
 * A060583  A ternary code related to the Tower of Hanoi.
 * https://oeis.org/A060583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 242, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A060583: Ternary code related to the Tower of Hanoi
 *
 * a(0) = 0
 * a(n) = 3 * a(floor(n/3)) + ((-a(floor(n/3)) - n) mod 3)
 *
 * The value is computed iteratively with memoisation so that each
 * term is evaluated only once, regardless of the order of calls.
 * The algorithm runs in O(n) time for the first call that reaches n
 * and is O(1) afterwards.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /*  Memoisation cache  */
    static seq_t *cache   = NULL;   /* cache[i] holds a(i) */
    static idx_t max_computed = 0;  /* highest index already computed */

    /* lazily initialise the cache for a(0) */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));   /* size 1  */
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = (seq_t)0;      /* a(0) = 0 */
        max_computed = 0;
    }

    /* extend the cache if necessary and compute missing values */
    if (n > max_computed) {
        /* grow cache to a power of two ≥ n+1 for cache-alignment */
        idx_t new_size = (max_computed == 0) ? 1 : max_computed;
        while (new_size <= n) new_size <<= 1;

        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) exit(EXIT_FAILURE);
        cache = new_cache;

        /* fill new entries starting just after max_computed */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            idx_t k = i / 3;          /* floor(i/3) */
            seq_t ak = cache[k];

            /* r = (-ak - i) mod 3  ==  (3 - (ak+i)%3) % 3  */
            idx_t sum_mod = (idx_t)((ak + (seq_t)i) % (seq_t)3);
            seq_t r = (seq_t)((3 - sum_mod) % 3);

            cache[i] = (seq_t)(3 * ak + r);
        }

        max_computed = n;
    }

    return cache[n];
}
