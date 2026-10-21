/*
 * A213382  Numbers n such that n^n mod (n + 2) = n.
 * https://oeis.org/A213382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 2554, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This implementation generates the A213382 sequence:
 * a(n) is the nth positive integer m such that m^m ≡ m (mod m+2).
 * The terms are computed by brute‑force search with caching, which
 * is efficient enough for n up to ~10⁴.
 */
#include <stdlib.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* static cache for already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t last_checked = 0; /* last m tested */

    /* grow cache until we have at least n terms */
    while (cache_len < n) {
        last_checked++;                         /* next candidate m */
        seq_t m  = (seq_t)last_checked;
        seq_t mod = m + 2;

        /* test the congruence */
        if (mod_pow(m, last_checked, mod) == m) {
            /* add to cache */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap ? cache_cap * 2 : 8);
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = m;
        }
    }

    return cache[n - 1];
}
