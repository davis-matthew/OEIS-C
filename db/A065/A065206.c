/*
 * A065206  Numbers which need one 'Reverse and Add' step to reach a
 *          palindrome.
 * https://oeis.org/A065206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers which need one 'Reverse and Add' step to reach a palindrome.
   a(n) = the nth smallest integer k such that k is not a palindrome and
   k + rev(k) is a palindrome.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached terms */
static size_t cache_len = 0;         /* number of cached terms */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static seq_t next_candidate = 9;     /* next integer to test (starts at 9 so first candidate is 10) */

/* Reverse a number in decimal. */ 
static seq_t reverse(seq_t x)
{
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x = x / 10;
    }
    return r;
}

/* Test whether a number is a palindrome. */
static bool is_palindrome(seq_t x)
{
    return reverse(x) == x;
}

/* Ensure cache has enough capacity for `needed` elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 32 : cache_cap * 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1, return 0 safely */

    /* Fill cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;
        /* Search for the next number that is not a palindrome itself,
           yet whose reverse-and-add result is a palindrome. */
        while (true) {
            if (!is_palindrome(cand)) {
                seq_t sum = cand + reverse(cand);
                if (is_palindrome(sum))
                    break;          /* cand satisfies the property */
            }
            ++cand;
        }
        /* Store the found term. */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len] = cand;
        ++cache_len;
        next_candidate = cand + 1;
    }

    return cache[n - 1];
}
