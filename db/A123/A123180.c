/*
 * A123180  Even positions of Sylvester's sequence A000058; the denominators
 *          of the (greedy) Egyptian fraction expansion of Cahen's constant.
 * https://oeis.org/A123180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)*(a(n-1)-1)*(a(n-1)*(a(n-1)-1)+1)+1 a(0) = 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cap    = 0;
    static size_t size   = 0;  /* number of elements stored (size >= 1 after init) */

    /* initialise cache with a(0) = 2 if needed */
    if (size == 0) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)2;
        size = 1;
    }

    /* extend cache up to n */
    if ((idx_t)size <= n) {
        /* grow capacity to at least n+1, doubling each step */
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap = new_cap == 0 ? 1 : new_cap * 2;

        if (new_cap > cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)  /* allocation failed – undefined behaviour, just keep old cache */
                return 0;
            cache = tmp;
            cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = size; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t term = prev * (prev - (seq_t)1) *
                         (prev * (prev - (seq_t)1) + (seq_t)1) + (seq_t)1;
            cache[i] = term;
        }
        size = (size_t)n + 1;
    }

    return cache[n];
}
