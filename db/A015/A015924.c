/*
 * A015924  Positive integers n such that 2^n == 16 (mod n).
 * https://oeis.org/A015924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A015924: a(n) = the n-th positive integer k such that 2^k ≡ 16 (mod k).
   This implementation searches for such k in increasing order and memoises the
   results so that subsequent calls with arbitrary n are fast. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 1; /* next integer to test */

    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */
    }

    /* Extend the cache until it contains at least n terms */
    while (cache_len < n) {
        uint64_t cand = next_candidate;
        seq_t cand_seq = (seq_t)cand;

        /* compute 2^cand (mod cand_seq) */
        seq_t r = mod_pow((seq_t)2, cand, cand_seq);
        seq_t target = ((seq_t)16 % cand_seq);

        if (r == target) {  /* cand satisfies the congruence */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) { free(cache); cache = NULL; return 0; }
                cache = tmp;
            }
            cache[cache_len++] = cand_seq;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
