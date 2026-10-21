/*
 * A093327  Smallest sequence of numbers having in their binary representation
 *          alternately more (>) or not-more (<=) binary ones than zeros.
 * https://oeis.org/A093327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   bitlen_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A093327: Smallest numbers whose binary ones/zeros
   alternate between "more ones" (> zeros) and "not more ones"
   (<= zeros).  a(0)=0, a(1)=1, a(2)=2, ...                   */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bitlen_seq.h"

/* Count 1-bits in a seq_t value */
static inline unsigned long popcnt_seq(seq_t v) {
    unsigned long c = 0;
    while (v) {
        v &= v - 1;
        ++c;
    }
    return c;
}

/* Cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Expand cache to at least size n+1 */
static void ensure_cache_size(idx_t n) {
    if ((size_t)n < cache_len) return;
    size_t newlen = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
    if (!tmp) abort();   /* OOM */
    cache = tmp;
    for (size_t i = cache_len; i < newlen; ++i) cache[i] = 0; /* init */
    cache_len = newlen;
}

/* Main function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    ensure_cache_size(n);
    if (cache[n] != 0) return cache[n];   /* already computed (except a(0)) */

    /* Compute terms up to n */
    for (idx_t k = 1; k <= n; ++k) {
        if (cache[k] != 0) continue;  /* already set */
        seq_t prev = (k == 1) ? (seq_t)0 : cache[k - 1];
        seq_t cand = prev + 1;
        int odd = (k & 1);   /* 1 if k is odd => > sign, else <= sign */
        while (1) {
            unsigned long ones = popcnt_seq(cand);
            unsigned long zeros = bitlen_seq(cand) - ones;
            if (odd) {
                if (ones > zeros) break;
            } else {
                if (ones <= zeros) break;
            }
            ++cand;
        }
        cache[k] = cand;
    }
    return cache[n];
}
