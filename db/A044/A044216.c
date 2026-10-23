/*
 * A044216  Numbers n such that string 3,5 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A044216 – numbers n such that the octal representation of n
    contains the substring “35” but n‑1 does not.
    The code uses a simple brute‑force search, keeps a global cache,
    and works for both fixed‑width integer builds and the arbitrary‑precision
    build (the seq_t type may be a class in C++).
*/
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Cache of already found terms */
static seq_t *cache = NULL;        /* array of terms  */
static size_t cache_len = 0;       /* number of terms stored  */
static size_t cache_cap = 0;       /* allocated capacity      */
static seq_t next_candidate = 1;   /* next integer to test    */

/* Return true if the octal representation of x contains the block 35.
   This function works for both integral and arbitrary‑precision seq_t. */
static bool has_35(seq_t x)
{
    if (x == 0)
        return false;

    seq_t y = x;          /* copy for iteration */
    seq_t prev = y % 8;    /* least‑significant digit */
    y /= 8;
    while (y > 0) {
        seq_t curr = y % 8;
        if (curr == 3 && prev == 5)
            return true;
        prev = curr;
        y /= 8;
    }
    return false;
}

/* Make sure the cache holds at least min_cap elements. */
static void grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term (1‑based index) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but harmless */

    /* Expand the cache until it contains a(n). */
    while (cache_len < (size_t)n) {
        if (has_35(next_candidate) && !has_35(next_candidate - 1)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
