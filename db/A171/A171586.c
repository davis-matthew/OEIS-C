/*
 * A171586  Squares divisible by at least two of their distinct digits >1.
 * https://oeis.org/A171586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   max_k_for_sq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A171586  Squares divisible by at least two distinct decimal digits > 1.
    The function generates terms lazily, caching them so that calls with
    arbitrary indices are fast and independent of order.  All arithmetic
    is performed using the seq_t type provided by the build. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/max_k_for_sq.h"

/* static cache for generated terms */
static seq_t *cache = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;
static idx_t last_k = 1;          /* next integer whose square we examine */

/* append value to the cache, enlarging as needed */
static void push_cache(seq_t val) {
    if (cache_cnt >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cache_cnt++] = val;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Build cache up to the desired index */
    while (cache_cnt < n) {
        /* Stop if squaring would overflow the underlying integer type */
        uint64_t maxk = max_k_for_sq();
        if (maxk != UINT64_MAX && last_k > maxk) break;

        seq_t x = (seq_t)last_k;
        seq_t sq = x * x;

        /* Collect distinct digits > 1 in the square */
        bool seen[10] = { false };
        int distinct = 0;
        seq_t tmp = sq;
        if (tmp != (seq_t)0) {
            while (tmp != (seq_t)0) {
                seq_t d_seq = tmp % (seq_t)10;
                int d = (int)d_seq;          /* d is in [0,9] */
                tmp = tmp / (seq_t)10;
                if (d > 1 && !seen[d]) {
                    seen[d] = true;
                    ++distinct;
                }
            }
        }

        /* Count how many of these digits divide the square */
        int divcnt = 0;
        for (int d = 2; d <= 9; ++d) {
            if (seen[d]) {
                seq_t dig = (seq_t)d;
                if (sq % dig == (seq_t)0) ++divcnt;
            }
        }

        if (divcnt >= 2) push_cache(sq);

        ++last_k;
    }

    return cache[n - 1];
}
