/*
 * A123116  Values y of solutions (x, y) to the Diophantine equation (x-y)^4 -
 *          8*x*y = 0 with x >= y.
 * https://oeis.org/A123116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*a(n-1) - 206*a(n-2) + 40*a(n-3) - a(n-4) with a(0)=0,
 *            a(1)=4, a(2)=192, a(3)=6860. The values are computed iteratively
 *            and cached for arbitrary‑precision build
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"  /* provides seq_t mul_small(seq_t, unsigned int) */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* extend cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;

        /* Reallocate the array (C++ compatible cast) */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) { /* allocation failed */
            abort();
        }
        cache = tmp;

        /* initialise new entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)0;
            } else if (i == 1) {
                cache[i] = (seq_t)4;
            } else if (i == 2) {
                cache[i] = (seq_t)192;
            } else if (i == 3) {
                cache[i] = (seq_t)6860;
            } else {
                /* recurrence: 40*a_{i-1} - 206*a_{i-2} + 40*a_{i-3} - a_{i-4} */
                seq_t term = mul_small(cache[i-1], 40);
                term = term - mul_small(cache[i-2], 206);
                term = term + mul_small(cache[i-3], 40);
                term = term - cache[i-4];
                cache[i] = term;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
