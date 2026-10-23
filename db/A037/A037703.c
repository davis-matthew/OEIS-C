/*
 * A037703  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,0,2.
 * https://oeis.org/A037703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*a(n-1) + a(n-4) - 5*a(n-5) Initial values: a(1)=1,
 *            a(2)=8, a(3)=40, a(4)=202, a(5)=1011
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: cache[0] unused, cache[1]..cache[cache_len] filled */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* On first call or if n > cache_len, grow cache */
    if (n > cache_len) {
        idx_t new_len = n;
        /* Allocate space for indices 0 .. new_len (size new_len+1) */
        seq_t *new_cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
        if (!new_cache) {
            abort(); /* allocation failure */
        }
        cache = new_cache;
        /* Ensure cache[0] is defined (unused) */
        cache[0] = (seq_t)0;

        /* Initial five terms */
        static const seq_t init[6] = { (seq_t)0, (seq_t)1, (seq_t)8,
                                       (seq_t)40, (seq_t)202, (seq_t)1011 };

        /* Fill from old cache size+1 up to new_len */
        for (idx_t i = cache_len + 1; i <= new_len; ++i) {
            if (i <= 5) {
                cache[i] = init[i];
            } else {
                cache[i] = (seq_t)5 * cache[i-1]
                           + cache[i-4]
                           - (seq_t)5 * cache[i-5];
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
