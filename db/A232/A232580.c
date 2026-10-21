/*
 * A232580  Number of binary sequences of length n that contain at least one
 *          contiguous subsequence 011.
 * https://oeis.org/A232580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 4*a(n-2) - a(n-3) + 2*a(n-4);
   Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=1. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;

    /* Allocate/resize cache as needed */
    if (cache == NULL) {
        size_t init_cap = 4;
        if (n + 1 > init_cap) init_cap = n + 1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cap = init_cap;
        /* store base cases */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        max_computed = 3;
    } else if (n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Compute up to n if necessary */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t v1 = cache[i - 1];
            seq_t v2 = cache[i - 2];
            seq_t v3 = cache[i - 3];
            seq_t v4 = cache[i - 4];
            cache[i] = 4 * v1 - 4 * v2 - v3 + 2 * v4;
        }
        max_computed = n;
    }

    return cache[n];
}
