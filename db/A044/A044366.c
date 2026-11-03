/*
 * A044366  Numbers n such that string 3,4 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044366: Numbers n such that the decimal representation of n contains
   the adjacent digits "34" but the representation of n-1 does not. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Test whether a decimal integer contains the adjacent digits 3,4
 * (in that order; the check is performed on the reversed digit
 * sequence, so we look for 4 followed by 3). */
static bool contains34(seq_t x)
{
    if (x < (seq_t)34) return false;
    seq_t prev = (seq_t)10; /* sentinel value that cannot equal 4 */
    while (x > 0) {
        seq_t cur = x % (seq_t)10;
        if (cur == (seq_t)3 && prev == (seq_t)4) return true;
        prev = cur;
        x /= (seq_t)10;
    }
    return false;
}

/* Dynamically allocated cache for the computed sequence values. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of values stored so far */
static size_t cache_cap  = 0;   /* allocated capacity of the cache */
static seq_t last_checked = (seq_t)33; /* candidate under consideration */

/* Ensure that the cache can hold at least 'need' elements.  Grow
 * the buffer using seq_realloc, which works for both integer and
 * arbitrary‑precision seq_t types. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp) abort();          /* memory exhaustion */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for index 0, placeholder */

    /* Advance the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        last_checked += (seq_t)1;
        if (contains34(last_checked) && !contains34(last_checked - (seq_t)1)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
    }
    return cache[(size_t)n - 1];
}
