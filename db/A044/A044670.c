/*
 * A044670  Numbers n such that string 4,2 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044670
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

/* The sequence A044670:
   a(n) is the n‑th integer n ≥ 1 such that the base‑9 representation of n
   contains the string "4,2" but the base‑9 representation of n+1 does not. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return true if x written in base 9 contains the substring 4,2. */
static bool has_42(seq_t x)
{
    if (x < (seq_t)2) return false;                     /* at least two digits */
    seq_t prev = x % (seq_t)9;                          /* least significant digit */
    x /= (seq_t)9;
    while (x > 0) {
        seq_t cur = x % (seq_t)9;                       /* next more significant digit */
        if (cur == (seq_t)4 && prev == (seq_t)2) return true;
        prev = cur;
        x /= (seq_t)9;
    }
    return false;
}

/* Ensure that two dynamically allocated arrays have at least 'needed'
   elements.  Cast the result of realloc (needed for C++). */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!newp) abort();                                 /* allocation failed */
    *arr  = newp;
    *cap  = new_cap;
}

/* Main function that returns the n-th term of sequence A044670. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                               /* undefined; fallback */

    static seq_t *cache   = NULL;                       /* stored sequence values */
    static size_t cache_size = 0;                       /* number of stored values */
    static size_t cache_cap   = 0;                       /* capacity of cache array */
    static seq_t current = 0;                            /* last integer checked */

    /* Build cache up to the n‑th element. */
    while (cache_size < (size_t)n)
    {
        current = current + (seq_t)1;                    /* next integer to test */
        if (has_42(current) && !has_42(current + (seq_t)1))
        {
            ensure_capacity(cache_size + 1, &cache_cap, &cache);
            cache[cache_size++] = current;
        }
    }
    return cache[n-1];
}
