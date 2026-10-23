/*
 * A173281  Let a(1) = 1. Given a(1), ..., a(2^t), find the least k such that
 *          a(1) + 2^k, a(2) + 2^k, ..., a(2^t) + 2^k are all composite and
 *          a(1) + 2^k > a(2^t). Then a(2^t+i) = a(i) + 2^k for all 1 <= i <=
 *          2^t.
 * https://oeis.org/A173281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A173281: a(1)=1.  For t≥0 let T=2^t.  Find the least k such that
   a(1)+2^k>a(T) and a(i)+2^k (1≤i≤T) are all composite.
   Then for 1≤i≤T set a(T+i)=a(i)+2^k.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* 1‑based cache; cache[0] unused. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* largest index computed */
    static idx_t cache_cap  = 0;  /* allocated capacity (including unused 0) */

    if (cache == NULL) {
        cache_cap = 16;                    /* initial capacity, 0..15 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[1] = (seq_t)1;                /* a(1)=1 */
        cache_size = 1;
    }

    /* Grow cache until it contains index n. */
    while (cache_size < n) {
        idx_t block = cache_size;          /* block size 2^t */
        idx_t k = 0;
        seq_t shift = 0;
        /* Find minimal k satisfying the block conditions. */
        for (;;) {
            shift = pow2(k);               /* 2^k in seq_t */
            if (shift <= cache[block] - cache[1]) { ++k; continue; }
            bool ok = true;
            for (idx_t i = 1; i <= block; ++i) {
                seq_t cand = cache[i] + shift;
                if (is_prime_big(cand)) { ok = false; break; }
            }
            if (ok) break;                 /* found suitable k */
            ++k;
        }

        /* Ensure enough capacity for the new block */
        idx_t new_size = cache_size + block;   /* new maximum index after adding */
        if (new_size + 1 > cache_cap) {        /* +1 because index 0 is unused */
            idx_t new_cap = cache_cap;
            while (new_cap <= new_size) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Append the shifted block */
        for (idx_t i = 1; i <= block; ++i) {
            cache[block + i] = cache[i] + shift;
        }
        cache_size = new_size;
    }

    return cache[n];
}
