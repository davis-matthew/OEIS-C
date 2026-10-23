/*
 * A029576  Number of permutations of an n-set containing a 9-cycle.
 * https://oeis.org/A029576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A029576:
 * a(0..8) = 0, a(9) = 40320.
 * For n > 9:
 *   if n % 9 != 0:    a(n) = n * a(n-1)
 *   else (n = 9*k, k > 1): a(n) = a(n-1) * S(k) / S(k-1)
 *   where S(1) = 1 and S(k) = 9*k*S(k-1) - (-1)^k. */
/* The implementation uses memoization and a simple loop
 * to compute values up to the maximum requested index. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static storage for memoization */
    static seq_t *cache = NULL;   /* cache[0..max] holds a(i) */
    static idx_t cache_len = 0;   /* allocated size of cache */
    static idx_t computed = 0;    /* highest index computed so far */

    /* static state for S(k) */
    static seq_t Sprev = 1;   /* S(k-1) for the last processed multiple of 9 */
    static idx_t Scur   = 1;   /* current k  (k of last processed multiple) */

    /* Make sure the cache is large enough */
    if (cache_len < n + 1) {
        /* allocate or grow */
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) return 0;  /* out of memory: return 0 as safe fallback */
        cache = new_cache;
        cache_len = n + 1;
    }

    /* Compute values up to requested n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i < 9) {
            /* first 9 terms are zero */
            cache[i] = 0;
        } else if (i == 9) {
            cache[9] = 40320;          /* 8! */
            /* no change to Sprev: remains 1 */
        } else if ((i % 9) != 0) {
            /* ordinary recurrence: a(n) = n * a(n-1) */
            cache[i] = (seq_t)i * cache[i - 1];
        } else {
            /* i = 9 * k, k > 1 */
            idx_t k = i / 9;               /* compute current k */
            /* compute S(k) from S(k-1) */
            seq_t Sk = Sprev * (seq_t)(9 * k); /* 9*k*S(k-1) */
            if (k & 1)    /* k odd -> add 1 */
                Sk = Sk + 1;
            else          /* k even -> subtract 1 */
                Sk = Sk - 1;
            /* compute a(i) = a(i-1) * Sk / Sprev */
            cache[i] = (cache[i - 1] * Sk) / Sprev;
            /* update Sprev for next multiple */
            Sprev = Sk;
        }
    }

    computed = n;    /* update highest computed index */

    return cache[n];
}
