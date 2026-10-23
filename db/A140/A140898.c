/*
 * A140898  Expansion of -x^2/(136*x^2+2*x-1).
 * https://oeis.org/A140898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 136*a(n-2), with a(1)=0, a(2)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t computed_up_to = 0; /* 0 means not initialized */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Lazy initialization of the cache */
    if (computed_up_to == 0) {
        size_t init_cap = 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure: safe fallback */
        cap = init_cap;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        computed_up_to = 2;
    }

    /* Grow capacity if needed */
    if (n > computed_up_to) {
        if (cap <= n) {
            size_t new_cap = cap ? cap * 2 : 16;
            while (new_cap <= (size_t)n) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;       /* allocation failure: safe fallback */
            cache = tmp;
            cap = new_cap;
        }
        /* Compute missing values up to n */
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i - 1] + (seq_t)136 * cache[i - 2];
        }
        computed_up_to = n;
    }

    return cache[n];
}
