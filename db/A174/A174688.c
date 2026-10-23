/*
 * A174688  All different products of not necessarily distinct terms of
 *          A001317.
 * https://oeis.org/A174688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sorted unique products of the terms of A001317 * (the
 *            Fermat numbers 3,5,17,257,65537,4294967297,…) starting with 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"

#define PF_COUNT 6

/* Fermat numbers F_k = 2^(2^k)+1 for k = 0..5 */
static const seq_t pf[PF_COUNT] = {
    (seq_t)3,          /* F0 = 3   */
    (seq_t)5,          /* F1 = 5   */
    (seq_t)17,         /* F2 = 17  */
    (seq_t)257,        /* F3 = 257 */
    (seq_t)65537,      /* F4 = 65537 */
    (seq_t)4294967297 /* F5 = 4294967297 */
};

static seq_t *seq_cache = NULL;
static size_t cache_len = 0;   /* number of terms cached */
static size_t cache_cap = 0;   /* capacity of seq_cache */

/* For each Fermat prime, the index of the smallest cached term that
 * has not yet been used as a multiplier by this prime. */
static size_t pos[PF_COUNT] = {0};

/* Ensure the cache can hold at least 'need' terms. */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_buf) abort();
    seq_cache = new_buf;
    cache_cap = new_cap;
}

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* unused but harmless */

    /* Bootstrap the cache if this is the first call. */
    if (cache_len == 0) {
        ensure_capacity(1);
        seq_cache[0] = (seq_t)1;
        cache_len = 1;
        /* pos[] already zero-initialised */
    }

    /* Generate terms until we have at least n of them. */
    if (cache_len < n) {
        ensure_capacity((size_t)n);
        while (cache_len < n) {
            seq_t cand[PF_COUNT];
            for (size_t i = 0; i < PF_COUNT; ++i) {
                cand[i] = seq_mul(seq_cache[pos[i]], pf[i]);
            }

            /* Find the minimal candidate. */
            seq_t min_val = cand[0];
            for (size_t i = 1; i < PF_COUNT; ++i) {
                if (cand[i] < min_val) min_val = cand[i];
            }

            /* Append the minimal value. */
            seq_cache[cache_len++] = min_val;

            /* Increment the indices of all primes that produced the minimal value. */
            for (size_t i = 0; i < PF_COUNT; ++i) {
                if (cand[i] == min_val) pos[i]++;
            }
        }
    }

    return seq_cache[n - 1];
}
