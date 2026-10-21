/*
 * A085937  Palindromes in A085936.
 * https://oeis.org/A085937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the nth integer n such that - n itself is a palindrome,
 *            and - let s be the number obtained by arranging the digits of n
 *            in increasing order; let p be the sum of squares of the digits
 *            of s; then s + p is a palindrome. The sequence starts with
 *            a(1)=1, a(2)=2, a(3)=191, ... as in OEIS A085937
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_len = 0;         /* current number of cached values */
static size_t cache_cap = 0;         /* allocated capacity of the cache */
static seq_t next_candidate = 1;     /* next integer to test */

/* Ensure sufficient capacity in the cache before inserting a new term. */
static void ensure_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        cache = new_arr;
        cache_cap = new_cap;
    }
}

/* Test whether a candidate integer satisfies the A085937 condition. */
static bool satisfies(seq_t n)
{
    /* First, n must itself be a palindrome. */
    if (!is_palindrome(n, (idx_t)10))
        return false;

    /* Extract digits of n, count them, and compute the sum of their squares. */
    unsigned char cnt[10] = {0};
    seq_t tmp = n;
    seq_t sumsq = 0;
    while (tmp > 0) {
        unsigned digit = (unsigned)(tmp % 10);
        cnt[digit]++;
        sumsq += (seq_t)digit * (seq_t)digit;
        tmp /= 10;
    }

    /* Build the ascending‑sorted number s from the counted digits. */
    seq_t s = 0;
    for (unsigned d = 0; d < 10; ++d) {
        for (unsigned c = 0; c < cnt[d]; ++c) {
            s = s * 10 + d;
        }
    }

    /* Compute s + sumsq and check whether it is a palindrome. */
    seq_t t = s + sumsq;
    return is_palindrome(t, (idx_t)10);
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for 0, return 0 */

    while (cache_len < (size_t)n) {
        if (satisfies(next_candidate)) {
            ensure_capacity();
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
