/*
 * A167077  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 1 * for 1 ≤ n ≤ 12: a(n) = 2 + 22 * (a(n-1) + ... +
 *            a(0)) * for n = 13: a(13) = 1 + 22 * (a(12)+...+a(1)) - 253 *
 *            a(0) * for n ≥ 14: a(n) = 22 * (a(n-1)+...+a(n-12)) - 253 *
 *            a(n-13) * (Derived from the rational generating function)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;    /* allocated size */
    static size_t sz  = 0;    /* number of values stored (next free index) */

    /* ensure capacity */
    if (n + 1 > cap) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;   /* out of memory */
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute terms up to n */
    while (sz <= (size_t)n) {
        seq_t val = 0;
        if (sz == 0) {                 /* a(0) */
            val = 1;
        } else if (sz >= 1 && sz <= 12) {
            /* a(sz) = 2 + 22 * sum_{k=1}^{sz} a(sz-k) */
            val = 2;
            for (idx_t k = 1; k <= sz; ++k)
                val += (seq_t)22 * cache[sz - k];
        } else if (sz == 13) {
            /* a(13) = 1 + 22 * sum_{k=1}^{12} a(13-k) - 253 * a(0) */
            val = 1;
            for (idx_t k = 1; k <= 12; ++k)
                val += (seq_t)22 * cache[13 - k];
            val -= (seq_t)253 * cache[0];
        } else { /* sz >= 14 */
            /* a(sz) = 22 * sum_{k=1}^{12} a(sz-k) - 253 * a(sz-13) */
            for (idx_t k = 1; k <= 12; ++k)
                val += (seq_t)22 * cache[sz - k];
            val -= (seq_t)253 * cache[sz - 13];
        }
        cache[sz] = val;
        ++sz;
    }
    return cache[(size_t)n];
}
