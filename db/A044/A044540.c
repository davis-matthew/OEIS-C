/*
 * A044540  Numbers n such that string 3,0 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A044540 – Numbers n such that the base‑7 representation of n
    contains the substring "30" while the representation of n+1 does
    not.  The function searches for the first n terms, storing them
    in a static cache that is grown on demand.  The cache is
    entirely independent of the order of calls to a(). */

#include <stdlib.h>
#include <stdbool.h>

/* Check whether a base‑7 expansion of x contains the digits 3,0
   in this order (most significant first). */
static inline bool has_30(seq_t x)
{
    /* At most 23 base‑7 digits fit in a 64‑bit number.  A buffer of
       32 is safe for arbitrary‑precision types as well. */
    unsigned char dig[32];
    int len = 0;

    if (x == 0)
        return false;

    /* Extract digits from least to most significant. */
    while (x != 0) {
        dig[len++] = (unsigned char)(x % 7);
        x /= 7;
    }

    /* Scan for 3 followed by 0 in correct order. */
    for (int i = len - 1; i > 0; --i) {
        if (dig[i] == 3 && dig[i - 1] == 0)
            return true;
    }
    return false;
}

/* Static cache storing found terms. */
static seq_t *cache      = NULL;   /* array of found terms           */
static size_t cache_len  = 0;      /* number of elements stored      */
static size_t cache_cap  = 0;      /* allocated capacity (in elems)  */
static seq_t  next_num   = 1;      /* next integer to test           */

/* Append a value to the cache, reallocating if necessary.  Cast
   the void* result of realloc to seq_t*. */
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – aborting is sufficient for
               the purposes of this library. */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Ensure that the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    /* n is guaranteed to be >= 1. */
    size_t target = (size_t)n;
    while (cache_len < target) {
        seq_t cur = next_num;
        if (has_30(cur) && !has_30(cur + (seq_t)1))
            cache_append(cur);
        ++next_num;
    }
}

/* Public function: returns the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
