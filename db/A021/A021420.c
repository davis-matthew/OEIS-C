/*
 * A021420  Decimal expansion of 1/416.
 * https://oeis.org/A021420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A021420: 
   a(n) = nth digit in the decimal expansion of 1/416 (period 6 after the first five digits) */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed digits, 0‑based indexed */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of digits stored */
    static idx_t cache_cap  = 0;   /* capacity of cache */
    static seq_t rem = 1;          /* current remainder, initial 1 for 1/416 */
    const seq_t denom = (seq_t)416;

    /* extend cache until it contains digit n */
    while (cache_size <= n) {
        /* grow cache capacity if necessary */
        if (cache_cap <= cache_size) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failed – return a fallback value */
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* long division step: rem = rem * 10, digit = rem / denom */
        rem *= (seq_t)10;
        seq_t digit = rem / denom;
        rem = rem % denom;

        cache[cache_size++] = digit;
    }

    return cache[n];
}
