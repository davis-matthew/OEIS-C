/*
 * A156922  Third right hand column (n-m=2) of the A156920 triangle.
 * https://oeis.org/A156922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            14*a(n-1)-75*a(n-2)+196*a(n-3)-263*a(n-4)+174*a(n-5)-45*a(n-6)
 *            with initial values a(0)=1, a(1)=15, a(2)=129, a(3)=877,
 *            a(4)=5280, a(5)=29658
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;            /* cached values */
    static idx_t cache_len = 0;            /* number of cached elements */

    /* initial terms */
    static const seq_t init[6] = {
        (seq_t)1,
        (seq_t)15,
        (seq_t)129,
        (seq_t)877,
        (seq_t)5280,
        (seq_t)29658
    };

    /* trivial case: answer is in the initial table */
    if (n < 6)
        return init[(size_t)n];

    /* grow cache if necessary */
    if (n + 1 > cache_len) {
        size_t needed = (size_t)(n + 1);
        if (cache == NULL) {
            cache = (seq_t *)malloc(needed * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        }
        /* initialise new positions that are still in the initial table */
        for (size_t i = cache_len; i < 6; ++i)
            cache[i] = init[i];

        /* compute recurrence for the rest */
        for (size_t i = 6; i < needed; ++i) {
            cache[i] = 14 * cache[i - 1]
                     - 75 * cache[i - 2]
                     + 196 * cache[i - 3]
                     - 263 * cache[i - 4]
                     + 174 * cache[i - 5]
                     - 45 * cache[i - 6];
        }
        cache_len = needed;
    }

    return cache[(size_t)n];
}
