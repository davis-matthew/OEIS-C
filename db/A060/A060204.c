/*
 * A060204  Square pyramorphic numbers: suppose the k-th square pyramidal
 *          number S(k) (A000330) ends in k; sequence gives value of S(k).
 * https://oeis.org/A060204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = S(k_n) where S(k)=k(k+1)(2k+1)/6 and * k_n is the nth
 *            positive integer such that S(k) % 10^d(k) == k, * with d(k) the
 *            number of decimal digits of k. This produces the *
 *            square‑pyramorphic numbers from OEIS A060204
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0, cache_cap = 0;
static idx_t next_k = 1;                 /* next integer k to test   */
static unsigned long long pow10_lim = 10;/* 10^digits bound for current k */
static unsigned long long pow10_val = 10;/* same as pow10_lim but used in % */

/* Grow the cache if needed. */
static void grow_cache(idx_t needed) {
    if (needed > cache_cap) {
        cache_cap = needed <= 4 ? 4 : needed;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Return the nth square‑pyramorphic number (1‑based). */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for n<1, but safe */
    /* Generate terms until we have at least n entries. */
    while (cache_len < n) {
        /* Update 10^d(k) bound when k grows to new digit length. */
        if (next_k >= pow10_lim) {
            pow10_lim *= 10;
            pow10_val *= 10;
        }

        seq_t k = (seq_t)next_k;
        seq_t s = k * (k + 1) * ((k << 1) + 1) / 6;   /* S(k)   */
        seq_t mod = s % (seq_t)pow10_val;             /* last d digits of S(k) */

        if (mod == k) {                               /* square‑pyramorphic? */
            grow_cache(cache_len + 1);
            cache[cache_len++] = s;
        }
        next_k++;                                     /* test next k */
    }
    return cache[n - 1];
}
