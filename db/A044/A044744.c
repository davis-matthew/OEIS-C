/*
 * A044744  Numbers n such that string 3,1 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer for which the decimal *
 *            representation of n contains the substring "31" but the
 *            representation * of n+1 does not. The function generates the
 *            terms by simple * scanning and memoises results so that calls
 *            with arbitrary order of * indices are efficient
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains31
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains31.h"

static seq_t *cache = NULL;      /* memoised terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* grow the cache array to hold at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (cache_cap < need) {
        size_t newcap = cache_cap ? cache_cap * 2 : 64;
        if (newcap < need) newcap = need;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* convert index to a C++/C size_t for array indexing */
    size_t target = (size_t)n;

    /* extend the memoised list until it contains the desired term */
    if (target > 0 && target > cache_len) {
        seq_t num = (cache_len == 0) ? 1 : cache[cache_len - 1] + 1;

        while (cache_len < target) {
            if (contains31(num)) {
                seq_t next = num + 1;
                if (!contains31(next)) {
                    ensure_cache(cache_len + 1);
                    cache[cache_len++] = num;
                }
            }
            num = num + 1;
        }
    }

    /* target is guaranteed to be >=1 by the sequence offset */
    return cache[target - 1];
}
