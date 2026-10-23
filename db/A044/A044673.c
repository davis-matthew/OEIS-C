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
 * Helpers:   contains_45
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
#include "../utils/contains_45.h"

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
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
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
