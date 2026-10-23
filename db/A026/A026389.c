/*
 * A026389  a(n) = number of integer strings s(0),...,s(n) counted by array T
 *          in A026386 that have s(n)=4; also a(n) = T(2n,n-2).
 * https://oeis.org/A026389
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) from OEIS A026389.
   Recurrence (valid for n ≥ 6):
          (n+2)·a(n) + 4·(-3n−2)·a(n−1) + 2·(24n−19)·a(n−2)
          + 4·(−18n+43)·a(n−3) + 35·(n−4)·a(n−4) = 0
   which simplifies to
          a(n) = [(12n+8)a(n−1) − (48n−38)a(n−2)
                 + (72n−172)a(n−3) − (35n−140)a(n−4)] / (n+2)
   Offset: 2 (a(2) = 1).  */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Under the offset, values are undefined; return 0 for safety. */
    if (n < 2)
        return (seq_t)0;

    /* Static cache for memoised values. */
    static seq_t *cache = NULL;      /* array of a values indexed by n */
    static idx_t cache_cap = 0;      /* current allocated capacity (>= n+1) */
    static idx_t max_cached = 1;     /* largest index whose value is stored */

    /* Allocate an initial buffer if needed. */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Make sure the buffer can hold index n. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (n + 1 > new_cap)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Seed the first few known terms (a(2) … a(5)). */
    if (max_cached < 5) {
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)49;
        cache[5] = (seq_t)272;
        max_cached = 5;
    }

    /* If the requested value is already computed, return it. */
    if (n <= max_cached)
        return cache[n];

    /* Compute successive values up to n using the recurrence. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* The recurrence is only valid for i ≥ 6. */
        if (i < 6) continue;  /* safety guard – shouldn't happen */

        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];

        seq_t num = (seq_t)(12 * i + 8) * t1
                    - (seq_t)(48 * i - 38) * t2
                    + (seq_t)(72 * i - 172) * t3
                    - (seq_t)(35 * i - 140) * t4;

        seq_t den = (seq_t)(i + 2);
        cache[i] = num / den;   /* division is exact */
    }

    max_cached = n;
    return cache[n];
}
