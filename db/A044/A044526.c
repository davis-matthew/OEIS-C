/*
 * A044526  Numbers n such that string 1,0 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044526: Numbers n such that the base‑7 representation of n
   contains the substring "10" but the base‑7 representation of n+1 does
   not.  The function returns the n‑th element of this sequence (a(1) = 7). */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether a base‑7 representation of v contains the substring "10".
   The digits are examined from least significant to most significant.
   The pattern 1‑0 is detected when a higher‑order digit equals 1 and
   the following lower‑order digit equals 0. */
static bool has10(seq_t v)
{
    if (v == 0)
        return false;
    unsigned char prev = (unsigned char)(v % 7);
    v /= 7;
    while (v > 0) {
        unsigned char cur = (unsigned char)(v % 7);
        if (cur == 1 && prev == 0)
            return true;
        prev = cur;
        v /= 7;
    }
    return false;
}

/* Return the n‑th term of A044526.  The function is memoised: a
   static cache is extended until it contains at least n elements. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for the sequence, but safe */

    static seq_t *cache = NULL;   /* cache of found terms */
    static size_t cache_len = 0;  /* number of cached terms */
    static size_t cache_cap = 0;  /* capacity of the cache */
    static seq_t next_to_test = 1;/* next candidate to examine */

    /* Extend the cache array if necessary */
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                 /* memory allocation failed */
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we reach the desired index */
    while (cache_len < n) {
        if (has10(next_to_test) && !has10(next_to_test + 1))
            cache[cache_len++] = next_to_test;
        ++next_to_test;
    }

    return cache[n - 1];
}
