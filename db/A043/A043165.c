/*
 * A043165  Numbers k such that 2 and 3 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th integer k (k ≥ 1) whose octal representation
 * contains the digits 2 and 3 as adjacent (either 23 or 32) while
 * the octal representation of k – 1 does not contain such a pair.
 * The function looks for successive integers until it has found
 * n terms, caching results so later calls may be answered in O(1). */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- static cache of already found terms ---------- */
static seq_t *cache      = NULL;   /* array of found numbers            */
static size_t cache_len  = 0;      /* number of elements stored         */
static size_t cache_cap  = 0;      /* current allocated capacity        */
static seq_t  max_checked = 0;     /* largest k examined so far         */

/* ---------- helper: test if octal representation of x contains 2 and 3 adjacent ----------
 * either 23 or 32. The digit vector holds least‑significant digit first. */
static bool has23(seq_t x)
{
    unsigned char digits[200];
    size_t len = 0;

    if (x == 0) {
        return false;
    }

    /* decompose x into octal digits (least‑significant first) */
    while (x > 0) {
        digits[len++] = (unsigned char)(x % 8u);
        x /= 8u;
    }

    if (len < 2) {
        return false;
    }

    /* look for 2 followed by 3 or 3 followed by 2 when reading from
     * most‑significant to least‑significant digit (digits[len‑1]..digits[0]) */
    for (size_t i = len; i-- > 1;) {                 /* i goes from len‑1 down to 1 */
        if ((digits[i]   == 2 && digits[i-1] == 3)   /* 23 */
            || (digits[i]   == 3 && digits[i-1] == 2))/* 32 */
        {
            return true;
        }
    }
    return false;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) {            /* undefined for n<1 */
        return (seq_t)0;
    }

    /* if already cached, return directly */
    if ((idx_t)n <= (idx_t)cache_len) {
        return cache[(size_t)(n-1)];
    }

    /* otherwise extend the cache until we have n terms */
    while (cache_len < (size_t)n) {
        /* advance candidate */
        max_checked++;

        /* test condition */
        if (has23(max_checked) && !has23(max_checked - 1)) {
            /* ensure enough capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failed – abort with 0 (undefined behaviour) */
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = max_checked;
        }
    }

    return cache[(size_t)(n-1)];
}
