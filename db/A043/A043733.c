/*
 * A043733  Numbers whose number of runs in their base-2 representation is
 *          congruent to 0 mod 6.
 * https://oeis.org/A043733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 9828, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Scan natural numbers and select those whose number of binary
 * runs (contiguous equal bits) is divisible by 6.  The runs of a number
 * x are given by runs(x) = popcount(x ^ (x >> 1)). */
 
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
 
static inline seq_t count_runs(seq_t x) {
    seq_t y = x ^ (x >> 1);
    seq_t cnt = 0;
    while (y) {
        y &= y - (seq_t)1;  /* clear lowest set bit */
        cnt++;
    }
    return cnt;
}
 
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 2;  /* start scanning from 1 */
 
static void ensure_capacity(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}
 
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for 0 but guard */
 
    /* Generate terms until we have at least n entries */
    if (cache_len < (size_t)n) {
        seq_t cand = next_candidate;
        while (cache_len < (size_t)n) {
            if (count_runs(cand) % 6 == 0) {
                ensure_capacity(cache_len + 1);
                cache[cache_len++] = cand;
            }
            cand++;
        }
        next_candidate = cand;
    }
    return cache[n - 1];
}
