/*
 * A182890  Number of (1,0)-steps of weight 1 at level 0 in all weighted
 *          lattice paths in L_n.
 * https://oeis.org/A182890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 (n = 0), 1 (n = 1), 2 (n = 2), 5 (n = 3), 14 (n = 4),
 *            and for n ≥ 5: a(n) = 2*a(n‑1) + a(n‑2) + 2*a(n‑3) – a(n‑4)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoized recurrence implementation */
    static seq_t *cache = NULL;   /* array of precomputed terms */
    static idx_t cache_len = 0;   /* current length of the array */

    /* Handle the first five terms directly */
    if (n < 5) {
        switch (n) {
        case 0: return (seq_t)0;
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        case 3: return (seq_t)5;
        case 4: return (seq_t)14;
        }
    }

    /* Extend the memoized array up to n if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* no memory – return 0 as a safe fallback */
        cache = new_cache;

        /* Initialize first five entries if this is the first call */
        if (cache_len == 0) {
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)2;
            cache[3] = (seq_t)5;
            cache[4] = (seq_t)14;
            cache_len = 5;
        }

        /* Compute subsequent terms up to n using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1]
                     +     cache[i - 2]
                     + 2 * cache[i - 3]
                     -     cache[i - 4];
        }

        cache_len = new_len;
    }

    return cache[n];
}
