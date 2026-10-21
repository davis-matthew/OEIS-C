/*
 * A079782  Final term of n-th row of triangle in A079784.
 * https://oeis.org/A079782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n ≥ 4, a(n) = lcm(1,2,…,n) – 1; * for n = 1,2,3 use the
 *            unique * consistent values 2, 5, 11, * obtained from the
 *            congruence rule * a(n) = min{ m | m ≡ (i-1) (mod i) for some i ≤
 *            n }
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Euclidean algorithm for arbitrary unsigned types. */
static inline seq_t gcd_seq_t(seq_t a, seq_t b) {
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Hard‑code the first three terms (computed from the congruence rule). */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;

    /* Static cache of lcm values: lcm_cache[i] = lcm(1,2,…,i).  lcm_cache[0] is unused
     * but initialised to 1 so that the recurrence works uniformly. */
    static seq_t *lcm_cache = NULL;
    static idx_t cache_size = 0;      /* currently allocated length (index of last valid element + 1) */

    if (cache_size == 0) {
        /* Reserve space for lcm_cache[0] which holds the identity 1. */
        cache_size = 1;
        lcm_cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!lcm_cache) abort();
        lcm_cache[0] = (seq_t)1;
    }

    /* Grow the cache up to the requested n.  All new entries are computed
     * using the recurrence  lcm(i) = lcm(i‑1) * i / gcd(lcm(i‑1), i). */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(lcm_cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        lcm_cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t prev   = lcm_cache[i - 1];
            seq_t i_sq   = (seq_t)i;
            seq_t g      = gcd_seq_t(prev, i_sq);
            lcm_cache[i] = (prev / g) * i_sq;   /* perform division first to avoid overflow */
        }
        cache_size = new_size;
    }

    return lcm_cache[n] - (seq_t)1;
}
