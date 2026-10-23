/*
 * A143903  A positive integer k is included if |(number of 0's in binary k) -
 *          (number of 1's in binary k)| divides k.
 * https://oeis.org/A143903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A143903: k such that |#0s - #1s| divides k (offset 1)
   Algorithm: brute‑force search with memoisation of results.
   For each candidate k, count 0s and 1s in its binary representation.
   If the absolute difference diff ≠ 0 and k % diff == 0, store k.
   Cache results so subsequent calls retrieve terms directly.
*/

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) {          /* offset 1: ignore n == 0 */
        return 0;
    }

    /* static cache and control variables */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t cand = 1;          /* next candidate to test */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* generate terms until we have at least n terms cached */
    while (cache_len < n) {
        seq_t k = cand;

        /* count ones and zeros */
        seq_t on = 0, zr = 0, tmp = k;
        while (tmp) {
            if (tmp & 1)
                on++;
            else
                zr++;
            tmp >>= 1;
        }

        /* compute absolute difference */
        seq_t diff = (on > zr) ? (on - zr) : (zr - on);

        /* test the condition */
        if (diff != 0 && (k % diff) == 0) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = k;
        }

        cand++;   /* move to next integer */
    }

    return cache[n - 1];
}
