/*
 * A044505  Numbers n such that string 3,4 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑6 representation
 *            contains the substring "34" while the next integer does not
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_used  = 0;
static idx_t  cache_cap   = 0;
static seq_t  next_candidate = (seq_t)1;   /* next integer to test */

/* Check whether base‑6 representation of v contains the substring 3,4 */
static bool has34(seq_t v)
{
    if (v == (seq_t)0)
        return false;
    int last = -1;             /* last less‑significant digit seen; -1 means none yet */
    seq_t six   = (seq_t)6;
    while (v != (seq_t)0) {
        int d = (int)(v % six);          /* current least‑significant digit (0‑5) */
        if (last == 4 && d == 3)         /* found 3 followed by 4 */
            return true;
        last = d;
        v /= six;
    }
    return false;
}

/* Append val to the cache, resizing if necessary */
static void push_cache(seq_t val)
{
    if (cache_used == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }
    cache[cache_used++] = val;
}

/* OEIS sequence A044505 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but avoid UB */

    /* generate terms until we have n in the cache */
    while (cache_used < n) {
        seq_t v = next_candidate;
        if (has34(v) && !has34(v + (seq_t)1))
            push_cache(v);
        next_candidate++;
    }
    return cache[n - 1];
}
