/*
 * A049086  Number of tilings of 4 X 3n rectangle by 1 X 3 rectangles.
 *          Rotations and reflections are considered distinct tilings.
 * https://oeis.org/A049086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) + a(n-3); Initial values: a(0)=1,
 *            a(1)=3, a(2)=13
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache to store computed values. */
    static seq_t *cache = NULL;     /* pointer to array of seq_t values */
    static idx_t capacity = 0;      /* current allocated size of cache */
    static idx_t computed = 0;      /* number of indices that have been computed (0..computed-1) */

    /* Resize cache if necessary. */
    if (n >= capacity) {
        size_t new_capacity = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) {          /* allocation failed – terminate */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        capacity = (idx_t)new_capacity;
    }

    /* Compute missing values up to index n. */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)3;
        } else if (i == 2) {
            cache[2] = (seq_t)13;
        } else {
            cache[i] = (seq_t)5 * cache[i-1] - (seq_t)3 * cache[i-2] + cache[i-3];
        }
    }
    computed = n + 1;   /* update number of computed indices */
    return cache[n];
}
