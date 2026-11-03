/*
 * A044313  Numbers k such that the string 6,8 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k such that the base‑9 representation of
 *            k contains the substring "68" but the base‑9 representation of
 *            k-1 does not. (OEIS A044313, offset 1)
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 8793 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Check whether a number in base 9 contains the consecutive digits 6,8. */
static bool has68(seq_t x)
{
    seq_t tmp = x;
    /* 200 digits is ample for n up to ~10^5 (used in the test). */
    uint8_t digits[200];
    size_t len = 0;
    while (tmp > (seq_t)0
#ifdef __cpp_alias_templates  /* Allow potential big integer operators. */
#endif
           )
    {
        digits[len++] = (uint8_t)(tmp % (seq_t)9);
        tmp /= (seq_t)9;
    }
    if (len < 2)
        return false;
    for (size_t i = 0; i + 1 < len; ++i)
        if (digits[i] == 8 && digits[i + 1] == 6)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    /* Static cache so that unsorted calls still work. */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t candidate = (seq_t)1;

    /* Grow the cache until it contains the nth element. */
    while (cache_size < (size_t)n) {
        /* Find the next valid number. */
        while (true) {
            if (has68(candidate) && !has68(candidate - (seq_t)1))
                break;
            ++candidate;
        }
        /* Allocate more space if needed. */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)
                abort(); /* Out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Store the found number and advance. */
        cache[cache_size++] = candidate;
        ++candidate;
    }
    return cache[n - 1];
}
