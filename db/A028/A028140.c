/*
 * A028140  Expansion of 1/((1-4x)(1-6x)(1-9x)(1-11x)).
 * https://oeis.org/A028140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 30*a(n-1) - 323*a(n-2) + 1470*a(n-3) - 2376*a(n-4)
 * Initial values: a(0)=1, a(1)=30, a(2)=577, a(3)=9090
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated capacity */
    static idx_t len   = 0;   /* number of terms stored (>=4 once initialized) */

    /* first call: initialise cache with the first four terms */
    if (len == 0) {
        /* ensure capacity at least 4, or n+1 if larger */
        cap = (n < 4) ? 4 : (n + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;   /* unlikely, but avoid crash */
        cache[0] = 1;
        cache[1] = 30;
        cache[2] = 577;
        cache[3] = 9090;
        len = 4;
    }

    /* if requested term already known, return it */
    if (n < len)
        return cache[n];

    /* enlarge cache if necessary */
    if ((idx_t)(n + 1) > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* unlikely */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t term1 = 30 * cache[i - 1];
        seq_t term2 = 1470 * cache[i - 3];
        seq_t sum1  = term1 + term2;

        seq_t term3 = 323 * cache[i - 2];
        seq_t term4 = 2376 * cache[i - 4];
        seq_t sum2  = term3 + term4;

        cache[i] = sum1 - sum2;   /* result is non‑negative for all i>=4 */
    }

    len = n + 1;   /* update number of stored terms */
    return cache[n];
}
