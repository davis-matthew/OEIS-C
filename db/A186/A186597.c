/*
 * A186597  Number of (n+3) X 10 0..3 arrays with every 4 X 4 subblock
 *          commuting with each horizontal and vertical neighbor 4 X 4
 *          subblock.
 * https://oeis.org/A186597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 23, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A186597
   Recurrence (n > 17):
     a(n) = 2*a(n-1) - a(n-2) + 3*a(n-4) - 6*a(n-5) + 3*a(n-6) + 3*a(n-8) - 3*a(n-9)
   Initial values:
     a(1) = 71095231
     a(2) = 1126074
     a(3) = 1889853
     a(4) = 5711805
     a(5) = 12563805
     a(6) = 17002221
     a(7) = 17395749
     a(8) = 17856813
     a(9) = 20158773
     a(10) = 23363757
     a(11) = 27162237
     a(12) = 33026037
     a(13) = 58823757
     a(14) = 91003557
     a(15) = 125475693
     a(16) = 167526981
     a(17) = 305320872
   The function maintains a memoised cache so any ordering of calls is supported. */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

#if defined(_MSC_VER)
#pragma warning(disable : 4996) /* disable deprecation warning for sscanf on MSVC */
#endif

/* string literals of the first 17 terms (index 0 unused) */
static const char *const init_str[18] = {
    "",                     /* 0 unused */
    "71095231",             /* 1 */
    "1126074",              /* 2 */
    "1889853",              /* 3 */
    "5711805",              /* 4 */
    "12563805",             /* 5 */
    "17002221",             /* 6 */
    "17395749",             /* 7 */
    "17856813",             /* 8 */
    "20158773",             /* 9 */
    "23363757",             /* 10 */
    "27162237",             /* 11 */
    "33026037",             /* 12 */
    "58823757",             /* 13 */
    "91003557",             /* 14 */
    "125475693",            /* 15 */
    "167526981",            /* 16 */
    "305320872"             /* 17 */
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *cache   = NULL;     /* cache[0] .. cache[n] */
    static size_t cache_cap = 0;      /* allocated size */
    static idx_t  computed = 0;       /* biggest n computed so far */

    /* Ensure the cache can hold indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise missing terms up to 17 */
    if (computed < 17) {
        idx_t start = (computed + 1 > 1) ? computed + 1 : 1;
        for (idx_t i = start; i <= (n < 17 ? n : 17); ++i) {
            cache[i] = seq_from_str(init_str[i]);
        }
        if (n <= 17) return cache[n];
        computed = 17;
    }

    /* Compute the recurrence for n > 17 */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = 0;
        val += (seq_t)2 * cache[i - 1];
        val -= cache[i - 2];
        val += (seq_t)3 * cache[i - 4];
        val -= (seq_t)6 * cache[i - 5];
        val += (seq_t)3 * cache[i - 6];
        val += (seq_t)3 * cache[i - 8];
        val -= (seq_t)3 * cache[i - 9];
        cache[i] = val;
    }
    computed = n;
    return cache[n];
}
