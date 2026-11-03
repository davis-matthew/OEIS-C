/*
 * A044673  Numbers n such that string 4,5 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044673: a(n) is the n‑th positive integer n such that the
   base‑9 representation of n contains the substring "45" and the
   base‑9 representation of n+1 does not. 
   The term a(n) is obtained by scanning integers in order and
   collecting those that satisfy the condition. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff the base‑9 representation of x contains the digits
   '4' followed by '5' (most significant → least significant). */
static bool contains_45(seq_t x)
{
    if (x == (seq_t)0)
        return false;

    /* 200 digits is more than enough for numbers encountered up to n≈10000. */
    char buf[200];
    size_t len = 0;
    seq_t tmp = x;
    while (tmp > (seq_t)0) {
        seq_t r = tmp % (seq_t)9;
        buf[len++] = (char)(((unsigned long)r) + '0'); /* LSD first */
        tmp = tmp / (seq_t)9;
    }
    for (size_t i = len; i > 1; --i) {          /* MSD → LSD order */
        if (buf[i - 1] == '4' && buf[i - 2] == '5')
            return true;
    }
    return false;
}

/* The types seq_t and idx_t are supplied by the build. */

static seq_t *cache = NULL;       /* stored sequence values   */
static size_t cache_len  = 0;     /* current number of terms  */
static size_t cache_cap  = 0;     /* allocated capacity       */
static seq_t  last_checked = 0;   /* last integer examined    */

/* Ensure the cache can store at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        cache = mem_resize(seq_t, cache, new_cap);
        cache_cap = new_cap;
    }
}

/* Append a new term to the cache and update the last checked value. */
static void add_term(seq_t val)
{
    ensure_capacity(cache_len + 1);
    cache[cache_len++] = val;
    last_checked = val;
}

/* Return the n‑th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined but harmless */

    /* Generate terms until we have at least n of them. */
    for (seq_t cur = last_checked + (seq_t)1; cache_len < n; ++cur) {
        seq_t next = cur + (seq_t)1;
        if (contains_45(cur) && !contains_45(next))
            add_term(cur);
    }
    return cache[n - 1];
}
