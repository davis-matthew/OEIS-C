/*
 * A043110  Numbers k such that 1 and 2 occur juxtaposed in the base-5
 *          representation of k but not of k-1.
 * https://oeis.org/A043110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043110 – Numbers k such that 1 and 2 occur juxtaposed in the
   base‑5 representation of k but not of k‑1.
   Computation: linear search with memoisation; each integer is
   tested for the adjacency pattern. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff n (in base 5) contains the substring "12" or "21". */
static bool has_adj_12_or_21(seq_t n)
{
    int prev = (int)(n % (seq_t)5);       /* least significant digit */
    n /= (seq_t)5;
    while (n > 0) {
        int curr = (int)(n % (seq_t)5);   /* next more significant digit */
        if ((curr == 1 && prev == 2) || (curr == 2 && prev == 1))
            return true;                 /* found juxtaposed 1 and 2 */
        prev = curr;
        n /= (seq_t)5;
    }
    return false;
}

/* Dynamic cache of found sequence elements */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t next_start  = (seq_t)1;   /* next integer to test */

/* Grow the cache array to at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap : 16;
    while (cap < needed) cap <<= 1;
    seq_t *new_arr = mem_new(seq_t, cap);
    if (!new_arr) exit(1);              /* abort on allocation failure */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
        mem_free(cache);
    }
    cache     = new_arr;
    cache_cap = cap;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined below offset */

    size_t target = (size_t)n;
    if (target > cache_len) {
        while (cache_len < target) {
            seq_t cand = next_start++;                     /* test successive integers */
            if (has_adj_12_or_21(cand) &&                /* cand has adjacency */
                !has_adj_12_or_21(cand - (seq_t)1)) {      /* but cand-1 does not */
                grow_cache(cache_len + 1);
                cache[cache_len++] = cand;
            }
        }
    }
    return cache[n - 1];
}
