/*
 * A044761  Numbers n such that string 4,8 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains48
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that decimal representation contains "48"
   but n+1 does not.  */
/* No simple OEIS formula – we discover terms by scanning integers. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains48.h"

/* Cache of found terms: cache[0] = a(1), cache[1] = a(2), ... */
static seq_t *cache   = NULL;
static size_t  len    = 0;       /* how many terms are cached */
static size_t  cap    = 0;       /* allocated capacity */
static idx_t   last   = 0;       /* last integer that has been checked */

/* Ensure the cache has room for at least one more term. */
static void ensure_capacity(void)
{
    if (len < cap) return;
    size_t new_cap = cap ? cap * 2 : 32;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();   /* unrecoverable memory allocation error */
    cache = new_cache;
    cap   = new_cap;
}

/* Return the n-th term (offset 1).  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety – real sequence starts at 1 */

    /* Grow the cache until it contains at least n terms. */
    while (len < n)
    {
        /* Advance candidate integer. */
        ++last;
        if (contains48(last) && !contains48(last + 1))
        {
            ensure_capacity();
            cache[len++] = (seq_t)last;
        }
    }
    return cache[n - 1];
}
