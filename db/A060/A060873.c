/*
 * A060873  Intrinsic 3-palindromes: n is an intrinsic k-palindrome if it is a
 *          k-digit palindrome in some base.
 * https://oeis.org/A060873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth intrinsic 3–palindrome (A060873)
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_3pal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A number m is an intrinsic 3–palindrome if there exists a base
   b ≥ 2 and digits d2 (1 ≤ d2 < b) and d1 (0 ≤ d1 < b) such that
   m = d2·(b²+1) + d1·b.  The function returns the value of the
   sequence at the 1‑based index n.  Values are cached to allow
   repeated calls with arbitrary order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_3pal.h"

/* ----- cache of computed terms ---- */
static seq_t *cache = (seq_t*)NULL;   /* ascending list of intrinsic 3–palindromes */
static size_t cache_cap = 0;          /* capacity in elements */
static size_t cache_cnt = 0;          /* number of stored terms */
static seq_t next_candidate = (seq_t)1;/* next integer to test */

/* ensure that the cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    /* realloc returns void* – cast explicitly for C++ */
    void *p = realloc(cache, new_cap * sizeof(seq_t));
    if (!p) exit(EXIT_FAILURE);        /* allocation failed */
    cache = (seq_t*)p;
    cache_cap = new_cap;
}

/* ----- OEIS sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined for 0; safe fallback */

    /* generate terms until we have at least 'n' of them */
    while (cache_cnt < (size_t)n) {
        seq_t m = next_candidate;

        /* find the next 3–palindrome */
        while (!is_3pal(m)) ++m;

        /* store it */
        ensure_cache_capacity(cache_cnt + 1);
        cache[cache_cnt] = m;
        ++cache_cnt;
        next_candidate = m + (seq_t)1;
    }

    return cache[n - 1];                /* 1‑based indexing */
}
