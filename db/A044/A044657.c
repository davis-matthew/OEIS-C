/*
 * A044657  Numbers n such that string 2,7 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044657:
   Numbers n such that the base‑9 representation of n contains the substring
   "27" but the base‑9 representation of n+1 does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether the base‑9 representation of x contains the substring "27". */
static bool has27(seq_t x)
{
    /* maximum digits for 128‑bit numbers is comfortably below 200 */
    unsigned char digits[200];
    int len = 0;
    while (x > 0) {
        digits[len++] = (unsigned char)(x % 9);
        x /= 9;
    }
    /* scan from most significant to least significant digit */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 2 && digits[i - 1] == 7)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of filled entries in cache */

    /* Return cached value if already computed. */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Ensure the cache can hold at least n entries. */
    size_t new_cap = cache_len ? (size_t)(cache_len * 2) : 16;
    if (new_cap < (size_t)n) new_cap = (size_t)n * 2;
    if (!cache) {
        cache = mem_new(seq_t, new_cap);
    } else if (new_cap > (size_t)cache_len) {
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
    }

    /* Start searching from the next number after the last cached term,
       or from 1 if the cache is empty. */
    seq_t cur = cache_len ? cache[cache_len - 1] + 1 : 1;

    /* Find terms until the cache contains n entries. */
    for (idx_t idx = cache_len + 1; idx <= n; ++idx) {
        for (;;) {
            if (has27(cur) && !has27(cur + 1)) {
                cache[cache_len++] = cur;
                ++cur;
                break;
            }
            ++cur;
        }
    }
    return cache[n - 1];
}
