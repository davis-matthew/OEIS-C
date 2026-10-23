/*
 * A041280  Numerators of continued fraction convergents to sqrt(153).
 * https://oeis.org/A041280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 4354*a(n-8) - a(n-16) (for n ≥ 16) Initial values:
 *            a(0)=12, a(1)=25, a(2)=37, a(3)=99, a(4)=235, a(5)=569,
 *            a(6)=804, a(7)=2177, a(8)=53052, a(9)=108281, a(10)=161333,
 *            a(11)=430947, a(12)=1023227, a(13)=2477401, a(14)=3500628,
 *            a(15)=9478657
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[16] = {
    (seq_t)12,    (seq_t)25,    (seq_t)37,    (seq_t)99,
    (seq_t)235,   (seq_t)569,   (seq_t)804,   (seq_t)2177,
    (seq_t)53052, (seq_t)108281,(seq_t)161333,(seq_t)430947,
    (seq_t)1023227,(seq_t)2477401,(seq_t)3500628,(seq_t)9478657
};

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* if already computed, simply return */
    if (n < cache_size) return cache[n];

    /* Need to extend cache to include index n */
    size_t new_size = (size_t)n + 1;

    /* Allocate or grow cache */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * new_size);
        /* initialise first min(new_size,16) entries */
        for (idx_t i = 0; i < new_size && i < 16; ++i)
            cache[i] = init_vals[i];
        cache_size = (idx_t)new_size;

        /* compute values from 16 up to n */
        for (idx_t i = 16; i < cache_size; ++i) {
            cache[i] = (seq_t)4354 * cache[i-8] - cache[i-16];
        }
    } else if (new_size > (size_t)cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) {
            /* Allocation failed; abort and leave function undefined for this call */
            return (seq_t)0;
        }
        cache = tmp;

        /* initialise any missing init_vals up to 15 before recurrence */
        for (idx_t i = cache_size; i < new_size && i < 16; ++i)
            cache[i] = init_vals[i];

        /* compute new terms (starting from max(16, cache_size)) */
        idx_t start = cache_size;
        if (start < 16) start = 16;
        for (idx_t i = start; i < new_size; ++i) {
            cache[i] = (seq_t)4354 * cache[i-8] - cache[i-16];
        }
        cache_size = (idx_t)new_size;
    }

    return cache[n];
}
