/*
 * A154250  a(n) = ( (9 + sqrt(7))^n - (9 - sqrt(7))^n )/(2*sqrt(7)).
 * https://oeis.org/A154250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 74*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 (so a(1) is the first term) */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initialise cache with a(0)=0, a(1)=1 */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* Extend cache if necessary */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* reallocation failure */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = (seq_t)18 * cache[i-1] - (seq_t)74 * cache[i-2];
        }
        cache_size = new_size;
    }

    return cache[n];
}
