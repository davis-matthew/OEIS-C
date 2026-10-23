/*
 * A143285  Number of binary words of length n containing at least one subword
 *          1000001 and no subwords 10^{i}1 with i<5.
 * https://oeis.org/A143285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 44, u32 n <= 86, u64 n <= 174,
 *            u128 n <= 351, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-6) - a(n-8) - a(n-13)   (n ≥ 14)
   Initial values a(0)…a(13) = 0,0,0,0,0,0,0,1,2,3,4,5,6,8. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for indices 0 .. 13. */
    static const seq_t base_init[14] = {
        0, 0, 0, 0, 0, 0, 0, 1,
        2, 3, 4, 5, 6, 8
    };

    /* Cached sequence and its current size. */
    static seq_t *cache      = NULL;
    static idx_t  cache_size = 0;
    static idx_t  max_comp   = 13;   /* last computed index */

    /* First call or grow cache to accommodate index n. */
    if (cache == NULL) {
        /* Ensure capacity for at least 14 base entries. */
        cache_size = (n > 13) ? n + 1 : 14;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure */

        /* Copy the known initial values. */
        for (idx_t i = 0; i < 14; ++i)
            cache[i] = base_init[i];
    } else if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failure */
        cache = tmp;
        cache_size = new_size;
        /* No need to reinitialize existing data. */
    }

    if (n < 14) return cache[n];

    /* Compute new terms up to n, using the recurrence. */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val = val + val;                   /* 2*a(i-1) */
        val = val - cache[i - 2];          /* - a(i-2) */
        val = val + cache[i - 6];          /* + a(i-6) */
        val = val - cache[i - 8];          /* - a(i-8) */
        val = val - cache[i - 13];         /* - a(i-13) */
        cache[i] = val;
    }
    max_comp = n;

    return cache[n];
}
