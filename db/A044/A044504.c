/*
 * A044504  Numbers n such that string 3,3 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer k ≥ 1 whose base‑6 representation
   contains the consecutive digits “33” while (k + 1) does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Check whether x in base 6 contains the substring 33.  */
static bool has33(seq_t x)
{
    int prev = -1;                     /* -1 indicates no previous digit yet */
    while (x != (seq_t)0) {
        int d = (int)(x % (seq_t)6);   /* least significant base‑6 digit */
        if (prev == 3 && d == 3) return true;
        prev = d;
        x /= (seq_t)6;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static state retained across calls */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_size = 0;
    static seq_t last_checked = 0;
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
        last_checked = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        seq_t cand = last_checked + (seq_t)1;
        last_checked = cand;

        if (has33(cand) && !has33(cand + (seq_t)1)) {
            /* Grow the cache if necessary */
            if (cache_size + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* unrecoverable allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
    }

    return cache[n - 1];
}
