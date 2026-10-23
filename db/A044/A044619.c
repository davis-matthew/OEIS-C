/*
 * A044619  Numbers n such that string 6,3 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044619: numbers n such that the base‑8 representation of n
   contains the adjacent digits 6,3 but the base‑8 representation of n+1
   does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Return true if the base‑8 representation of x contains the substring
   "63" (digits 6 followed by 3). */
static bool has63(seq_t x)
{
    if (x < 8) return false;               /* a single digit cannot contain "63" */
    unsigned int prev = (unsigned int)(x % 8);
    x /= 8;
    while (x != 0)
    {
        unsigned int cur = (unsigned int)(x % 8);
        if (cur == 6 && prev == 3) return true;
        prev = cur;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset is irrelevant */

#ifdef __cplusplus
    /* C++ build: use std::vector for dynamic storage. */
    static std::vector<seq_t> cache;
    while (cache.size() < n)
    {
        static seq_t last_checked = 0;
        ++last_checked;
        if (has63(last_checked) && !has63(last_checked + 1))
            cache.push_back(last_checked);
    }
    return cache[n-1];
#else
    /* C build: manual dynamic array with realloc. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;

    /* Initialise the cache if first call. */
    if (cache_cap == 0)
    {
        cache_cap = 64;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        ++last_checked;
        if (has63(last_checked) && !has63(last_checked + 1))
        {
            if (cache_len == cache_cap)
            {
                cache_cap <<= 1;                      /* double capacity */
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = last_checked;
        }
    }
    return cache[n-1];
#endif
}
