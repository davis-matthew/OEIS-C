/*
 * A172186  Numbers k such that k, k+1 and 2*k+1 are squarefree.
 * https://oeis.org/A172186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th smallest integer k such that k, k+1 and 2k+1 are
   square‑free (offset 1, so a(1) = 1).  The function uses a simple
   incremental search with memoisation.  All tests for square‑freeness
   are performed on uint64_t representations; this is safe because the
   values required for n ≤ 10 000 are far below 2^64. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* test whether a 64‑bit integer is squarefree by factorisation */
static inline bool is_squarefree64(uint64_t v)
{
    if (v <= 3) return true;          /* 1,2,3 are square‑free */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64(v, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] > 1)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t len = 0;          /* number of terms currently stored */
    static uint64_t next_k = 1;    /* next integer to test */

    /* initialise storage */
    if (cache == NULL) {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend cache until it contains at least n terms */
    while (len < n) {
        uint64_t k64   = next_k;
        uint64_t k1_64 = k64 + 1;
        uint64_t k2_64 = 2 * k64 + 1;

        if (is_squarefree64(k64) &&
            is_squarefree64(k1_64) &&
            is_squarefree64(k2_64)) {
            /* grow cache if needed */
            if (len == cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = (seq_t)k64;
        }
        ++next_k;
    }

    return cache[n - 1];   /* a(1) is stored at cache[0] */
}
