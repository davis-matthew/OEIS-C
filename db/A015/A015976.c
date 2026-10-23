/*
 * A015976  One iteration of Reverse and Add is needed to reach a palindrome.
 * https://oeis.org/A015976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 3192, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of A015976: numbers n for which n + reverse(n)
   is a palindrome (including palindromic n).  A(n) is the n-th
   such integer. */
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/reverse_decimal.h"
#include "../utils/is_palindrome.h"

/*----- cache for the sequence values ------------------------------------*/
static seq_t *cache = NULL;     /* array of computed terms            */
static size_t cache_cap = 0;    /* allocated size of cache            */
static size_t cache_len = 0;    /* number of elements actually stored */
static idx_t  checked_upto = 0; /* largest integer examined so far     */

/* Ensure the cache has at least min_cap elements available. */
static void cache_grow(size_t min_cap) {
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < min_cap)
            new_cap = min_cap;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) { /* memory allocation failed */
            abort();  /* this is a fatal error for the sequence generator */
        }
        cache = newp;
        cache_cap = new_cap;
    }
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n terms. */
    while (cache_len < (size_t)n) {
        checked_upto++;
        seq_t rev  = reverse_decimal((seq_t)checked_upto);
        seq_t sum  = (seq_t)checked_upto + rev;
        if (is_palindrome(sum, 10)) {
            cache_grow(cache_len + 1);
            cache[cache_len++] = (seq_t)checked_upto;
        }
    }
    return cache[(size_t)n - 1];
}
