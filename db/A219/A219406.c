/*
 * A219406  Unmatched value maps: number of n X 4 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal or
 *          antidiagonal neighbor in a random 0..2 n X 4 array.
 * https://oeis.org/A219406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 57*a(n-2) + 96*a(n-3) + 27*a(n-4) - 11*a(n-5)
 *            - 4*a(n-6) Initial values: a(1)=7, a(2)=98, a(3)=1199,
 *            a(4)=14737, a(5)=181089, a(6)=2225293
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* guard for accidental zero index */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;  /* number of filled elements */
    static size_t cache_cap  = 0;  /* allocated capacity */

    size_t needed = (size_t)n;

    /* allocate or grow cache if necessary */
    if (needed > cache_cap) {
        size_t new_cap = needed > 0 ? needed : 1;
        seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = ptr;
        cache_cap = new_cap;
    }

    /* initialise first 6 terms on first use */
    if (cache_size < 6 && cache_size < needed) {
        static const seq_t init[6] = {
            (seq_t)7,
            (seq_t)98,
            (seq_t)1199,
            (seq_t)14737,
            (seq_t)181089,
            (seq_t)2225293
        };
        while (cache_size < 6 && cache_size < needed) {
            cache[cache_size] = init[cache_size];
            cache_size++;
        }
    }

    /* compute terms up to required index using the recurrence */
    while (cache_size < needed) {
        size_t k = cache_size;           /* index of term to compute (0‑based) */
        seq_t val =
            (seq_t)7  * cache[k-1] +
            (seq_t)57 * cache[k-2] +
            (seq_t)96 * cache[k-3] +
            (seq_t)27 * cache[k-4] -
            (seq_t)11 * cache[k-5] -
            (seq_t)4  * cache[k-6];
        cache[cache_size++] = val;
    }

    return cache[needed - 1];
}
