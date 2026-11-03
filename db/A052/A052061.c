/*
 * A052061  Numbers k such that decimal expansion of k^2 contains no
 *          palindromic substring except single digits.
 * https://oeis.org/A052061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1046, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 86 terms through n = 9472 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A052061:
   a(n) = the n-th integer k whose square contains no palindromic
   decimal substring of length ≥ 2.  The sequence is indexed with
   offset 1, so a(1) is the first term (0).  The computation is
   performed by a brute‑force search, memoising found terms. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether the square of k contains any palindromic decimal
   substring of length ≥ 2.  The function uses only the required
   arithmetic operators and stdio functions. */
static bool is_good_candidate(seq_t k)
{
    seq_t sq = k * k;                 /* square of k */

    /* Extract digits of sq into a small stack array. */
    uint8_t digits[64];               /* enough for very large numbers */
    size_t len = 0;
    seq_t tmp = sq;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    /* Reverse to most‑significant‑digit first. */
    for (size_t i = 0, j = len - 1; i < j; ++i, --j) {
        uint8_t t = digits[i];
        digits[i] = digits[j];
        digits[j] = t;
    }

    /* Test all substrings of length ≥ 2 for being palindromic. */
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            size_t left = i, right = j;
            bool pal = true;
            while (left < right) {
                if (digits[left] != digits[right]) {
                    pal = false;
                    break;
                }
                ++left;
                --right;
            }
            if (pal) return false;  /* forbidden palindrome found */
        }
    }
    return true;
}

/* Function required by the build system.  Returns the n‑th term. */
seq_t a(idx_t n)
{
    /* Static cache of already found terms.  Size grows as needed. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Initialise cache on first call. */
    if (cache_len == 0) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return 0;            /* allocation failure */
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Ensure cache can hold at least n terms. */
    if (n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;             /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate new terms until the cache contains at least n entries. */
    while (cache_len < n) {
        seq_t cand = cache[cache_len - 1] + 1;
        while (!is_good_candidate(cand)) {
            /* Avoid infinite loop on overflow of fixed size types. */
            if (cand == 0) return 0;
            ++cand;
        }
        cache[cache_len] = cand;
        ++cache_len;
    }

    return cache[n - 1];
}
