/*
 * A044793  Numbers n such that string 8,0 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_80
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth natural number that contains the substring “80”
   in its decimal representation while its successor n+1 does not contain
   that substring. The function is computed by a simple brute‑force search
   with memoisation, which is adequate for n up to about 10 000. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_80.h"

seq_t a(idx_t n)
{
    /* Static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;        /* number of terms stored */
    static seq_t last_candidate = 0;    /* last examined number */

    /* Ensure cache has room for n terms */
    if (cache_cnt < n) {
        if (cache == NULL) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }
        while (cache_cap < n) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        /* Generate terms until we have n of them */
        while (cache_cnt < n) {
            /* Find next candidate > last_candidate */
            seq_t cand = last_candidate + 1;
            while (true) {
                if (has_80(cand) && !has_80(cand + 1)) {
                    cache[cache_cnt++] = cand;
                    last_candidate = cand;
                    break;
                }
                cand = cand + 1;
            }
        }
    }
    return cache[n - 1];  /* 1‑based indexing */
}
