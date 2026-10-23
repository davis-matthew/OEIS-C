/*
 * A044261  Numbers k such that string 1,1 occurs in the base 9 representation
 *          of k but not of k-1.
 * https://oeis.org/A044261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements OEIS A044261.
   It finds, for a given n ≥ 1, the n‑th integer k such that
   the base‑9 representation of k contains the substring “11”
   while the base‑9 representation of k‑1 does not.
   The function is memoised and works in O(n) time on each call. */

#include <stdlib.h>
#include <stdbool.h>

static bool has11(seq_t x)
{
    seq_t t = x;
    int prev = -1;               /* least‑significant digit first */
    while (t > 0) {
        int cur = (int)(t % 9);
        if (prev == 1 && cur == 1)
            return true;
        prev = cur;
        t /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for n=0, return 0 as safety */
        return 0;

    /* Cached results */
    static seq_t *cache      = NULL;
    static idx_t  cache_len  = 0;
    static size_t cache_cap  = 0;

    /* State for incremental generation */
    static seq_t  last_k      = 0;
    static bool   last_has    = false;

    /* Expand cache until we have at least n elements */
    while (cache_len < n) {
        last_k++;                          /* next integer to test */
        bool cur_has = has11(last_k);
        if (cur_has && !last_has) {        /* first appearance of “11” */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;

#ifdef __cplusplus
                /* In C++ use new[] to ensure constructors are called */
                seq_t *tmp = new seq_t[new_cap];
                if (!tmp) abort();      /* allocation failed */
                /* copy existing contents */
                for (size_t i = 0; i < cache_len; ++i)
                    tmp[i] = cache[i];
                delete[] cache;          /* free old array */
                cache = tmp;
#else
                /* In C use realloc() */
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();      /* allocation failed */
                cache = tmp;
#endif
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_k;
        }
        last_has = cur_has;
    }

    return cache[(size_t)(n - 1)];          /* cache is zero‑based    */
}
