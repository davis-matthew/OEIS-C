/*
 * A042856  Numerators of continued fraction convergents to sqrt(959).
 * https://oeis.org/A042856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(n) = 1920*a(n-4) - a(n-8)  (n ≥ 8)
   Initial terms: a(0)=30, a(1)=31, a(2)=929, a(3)=960,
                   a(4)=58529, a(5)=59489, a(6)=1783710, a(7)=1843199 */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static const seq_t base[8] = {
    30, 31, 929, 960, 58529, 59489, 1783710, 1843199
};

seq_t a(idx_t n)
{
    /* Grow cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)                       /* allocation failed */
            return (seq_t)0;
        cache = new_cache;
        /* Initialise new entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 8)
                cache[i] = base[i];
            else
                cache[i] = (seq_t)1920 * cache[i - 4] - cache[i - 8];
        }
        cache_size = new_size;
    }
    return cache[n];
}
