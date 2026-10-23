/*
 * A174196  Those positive integers that are both in sequence A166934 and in
 *          sequence A166935.
 * https://oeis.org/A174196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A174196: numbers whose binary representations
 * contain at least two longest palindromic substrings of
 * equal length, one homogeneous and one heterogeneous. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Return the bit (0 or 1) at position pos of x.  
 * Position 0 is the least‑significant bit. */
static inline int bit_at(seq_t x, idx_t pos) {
    return (int)((x >> pos) & 1);
}

/* Test whether x is a member of the sequence. */
static bool is_candidate(seq_t x) {
    if (x < 3) return false;          /* 1 and 2 are impossible */

    /* Compute bit length of x. */
    idx_t bitlen = 0;
    seq_t tmp = x;
    while (tmp) {
        tmp >>= 1;
        bitlen++;
    }

    if (bitlen == 0) return false;    /* safety */

    idx_t maxLen = 0;
    bool hasHomog = false, hasInhomog = false;

    /* Check all palindromic substrings by expanding around each
     * center (odd and even length). */
    for (idx_t center = 0; center < bitlen; ++center) {
        /* Odd‑length palindromes. */
        int left  = (int)center;
        int right = (int)center;
        while (left >= 0 && right < (int)bitlen &&
               bit_at(x, left) == bit_at(x, right)) {
            left--;  right++;
        }
        left++; right--;
        idx_t len = (idx_t)(right - left + 1);
        if (len > 0) {
            bool mono = true;
            int first = bit_at(x, left);
            for (int i = left; i <= right; ++i)
                if (bit_at(x, i) != first) { mono = false; break; }
            if (len > maxLen) {
                maxLen = len;
                hasHomog = mono;
                hasInhomog = !mono;
            } else if (len == maxLen) {
                hasHomog    = hasHomog    || mono;
                hasInhomog  = hasInhomog  || !mono;
            }
        }

        /* Even‑length palindromes. */
        left  = (int)center;
        right = (int)center + 1;
        while (left >= 0 && right < (int)bitlen &&
               bit_at(x, left) == bit_at(x, right)) {
            left--;  right++;
        }
        left++; right--;
        len = (idx_t)(right - left + 1);
        if (len > 0) {
            bool mono = true;
            int first = bit_at(x, left);
            for (int i = left; i <= right; ++i)
                if (bit_at(x, i) != first) { mono = false; break; }
            if (len > maxLen) {
                maxLen = len;
                hasHomog = mono;
                hasInhomog = !mono;
            } else if (len == maxLen) {
                hasHomog    = hasHomog    || mono;
                hasInhomog  = hasInhomog  || !mono;
            }
        }
    }

    return (maxLen > 0) && hasHomog && hasInhomog;
}

/* Main sequence function with memoisation. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_test = 1;   /* integer to test next */

    /* Initialise cache if first call. */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len >= cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Populate cache until it contains at least n terms. */
    while (cache_len < n) {
        while (!is_candidate((seq_t)next_test))
            ++next_test;
        cache[cache_len++] = (seq_t)next_test;
        ++next_test;
    }

    return cache[n - 1];
}
