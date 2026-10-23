/*
 * A132064  Numbers multiplied by 4 and written backwards.
 * https://oeis.org/A132064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse(4 * a(n-1)), a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/reverse.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t cached_n = 0;

    if (n == 0) {
        /* The sequence is not defined for n = 0 (offset is 1). */
        return 0;
    }

    /* Initial allocation and first element. */
    if (cache == NULL) {
        size_t init_cap = 2;                           /* index 0 unused, index 1 stored */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return 0;                 /* allocation failure: signal error */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cap = init_cap;
        cached_n = 1;
    }

    /* Extend cache if required. */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;                  /* allocation failure: signal error */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute values that are still missing, iteratively. */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t prod = cache[i - 1] * (seq_t)4;
        cache[i] = reverse(prod);
    }
    cached_n = n;

    return cache[n];
}
