/*
 * A044468  Numbers n such that string 2,0 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer whose base‑5 representation contains
 *            the substring "20" while the next integer does not. The function
 *            builds the sequence on demand, caching previously found values
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Test whether the base‑5 representation of x contains the
   substring 2,0 (most significant digit first). */
static bool has20(seq_t x)
{
    int prev = -1;          /* previous digit, -1 if none yet */
    while (x > 0) {
        seq_t d = x % 5;    /* least significant digit */
        if (prev == 0 && d == 2)
            return true;
        prev = (int)d;
        x /= 5;
    }
    return false;
}

/* Append a value to the dynamic array, enlarging if necessary. */
static void push_cache(seq_t **cache, size_t *cap, size_t *len, seq_t value)
{
    if (*len == *cap) {
        *cap = (*cap == 0) ? 32 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*cache, *cap * sizeof(seq_t));
        *cache = tmp; /* realloc is safe; tmp is non‑NULL here */
    }
    (*cache)[*len] = value;
    (*len)++;
}

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next = 0; /* next integer to test, starts at 0 */

    while (cache_len < (size_t)n) {
        ++next;                 /* examine the next integer */
        if (has20(next) && !has20(next + 1)) {
            push_cache(&cache, &cache_cap, &cache_len, next);
        }
    }
    return cache[n - 1];
}
