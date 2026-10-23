/*
 * A154968  a(n) = 4*a(n-1) + 12*a(n-2), n>2 with a(0)=1, a(1)=1, a(2)=7.
 * https://oeis.org/A154968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=7; a(n)=4*a(n-1)+12*a(n-2) for n>2 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Handle the base cases directly. */
    if (n < 3) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)1;
            default: return (seq_t)7;   /* n == 2 */
        }
    }

    /* Initialise the cache with the first three terms if necessary. */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
    }

    /* If the requested n is beyond the cache, grow it and fill in new entries. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = 4 * cache[i-1] + 12 * cache[i-2];
        }
        cache_size = new_size;
    }

    return cache[n];
}
