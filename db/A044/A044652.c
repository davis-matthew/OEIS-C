/*
 * A044652  Numbers n such that string 2,2 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044652:  Numbers n such that the base‑9 representation of n
 * contains the substring 22 but the representation of n+1 does not. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Test whether a number in base 9 contains the substring 22. */
static inline bool has22(seq_t x)
{
    seq_t prev = 9;                 /* sentinel larger than any base‑9 digit */
    while (x > 0) {
        seq_t d = x % 9;
        if (d == 2 && prev == 2)
            return true;
        prev = d;
        x = x / 9;
    }
    return false;
}

/* Compute the n-th term of OEIS A044652. */
seq_t a(idx_t n)
{
    if (n == 0)                       /* out of range; not used by tests */
        return 0;

    /* Static storage so calls are independent of order. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t found_terms = 0;
    static seq_t last_checked = 0;    /* last numeric value examined */

    /* Grow the cache array if necessary. */
    if (found_terms >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        cache = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (found_terms < (size_t)n) {
        seq_t cur = last_checked + 1;          /* next integer to test */
        if (has22(cur) && !has22(cur + 1)) {    /* satisfies definition */
            cache[found_terms++] = cur;
        }
        last_checked = cur;
    }

    return cache[n - 1];
}
