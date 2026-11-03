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
 * Validated: standard, pass, 79 terms through n = 917 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

static inline bool is_3pal(seq_t m)
{
    const seq_t FIVE = 5;
    const seq_t ZERO = 0;
    if (m < FIVE) return false;        /* minimal intrinsic 3–palindrome is 5 */

    for (seq_t b = 2; ; ++b) {
        const seq_t bb = b * b;        /* b² */
        if (bb > m) break;            /* larger bases impossible */

        const seq_t d2 = m % b;        /* leading digit */
        if (d2 == ZERO) continue;      /* leading digit cannot be 0 */

        const seq_t prod = d2 * (bb + (seq_t)1); /* d2·(b²+1) */
        if (prod > m) continue;

        const seq_t diff = m - prod;
        if (diff % b != ZERO) continue; /* d1 must be integer */

        const seq_t d1 = diff / b;
        if (d1 < b) return true;         /* valid representation found */
    }
    return false;
}

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
    void *p = mem_resize(seq_t, cache, new_cap);
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
