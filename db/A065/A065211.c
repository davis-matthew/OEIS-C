/*
 * A065211  Numbers which need six 'Reverse and Add' steps to reach a
 *          palindrome.
 * https://oeis.org/A065211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This implementation computes the OEIS sequence A065211:
 * numbers that require exactly six Reverse‑and‑Add steps to become a palindrome.
 * The function enumerates candidates, performs the reverse‑and‑add process
 * six times, and records those that satisfy the condition. The generated
 * terms are cached to allow arbitrary‑ordered calls.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/reverse_decimal.h"
#include "../utils/is_palindrome.h"

static inline bool needs_six_steps(seq_t x)
{
    /* Return true iff the number x is NOT a palindrome and
     * reaches a palindrome after exactly six Reverse‑and‑Add steps. */
    seq_t cur = x;
    for (idx_t step = 0; step < 6; ++step) {
        if (is_palindrome(cur, 10))       /* already a palindrome – exclude */
            return false;
        cur = cur + reverse_decimal(cur);
    }
    return is_palindrome(cur, 10);        /* true only if palindrome after 6 steps */
}

seq_t a(idx_t n)
{
    /* Static state: memoised list of terms and the next candidate to test. */
    static seq_t *cache = NULL;          /* array of found terms */
    static size_t cache_cap = 0;         /* capacity of cache */
    static size_t cache_len = 0;         /* number of terms stored */
    static seq_t next_candidate = 1;     /* next integer to test */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        if (needs_six_steps(next_candidate)) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 8;
                /* Use a cast so the code compiles under both C and C++. */
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();     /* out of memory – terminate */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        /* Move to the next integer candidate. */
        ++next_candidate;
    }

    return cache[n - 1];
}
