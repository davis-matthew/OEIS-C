/*
 * A050429  Numbers for which in base 2 the least number of digits that can be
 *          removed to leave a base 2 palindromic number (beginning with 1) is
 *          5.
 * https://oeis.org/A050429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find numbers that need exactly five deletions to reach a binary
   palindrome that starts with 1. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_BITS 128                     /* enough for the required range   */
static bool has_delete_five(seq_t x)
{
    /* Bit length of x.  For small integral types x fits in at most 64 bits. */
    unsigned len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        tmp >>= 1;
        ++len;
    }
    if (len <= 5 || len > MAX_BITS)   /* must delete at least five digits */
        return false;

    /* Extract bits into an array, most significant first. */
    unsigned char bits[MAX_BITS];
    for (unsigned i = 0; i < len; ++i) {
        /* shift may be up to 127 for seq_t being a big‑int class, which is fine. */
        seq_t mask = ((seq_t)1) << (len - 1 - i);
        bits[i] = (x & mask) ? 1 : 0;
    }

    /* DP table: L[i][j] = length of the longest palindromic subsequence
       in bits[i .. j] (inclusive).  The table is reused across calls. */
    static uint8_t L[MAX_BITS][MAX_BITS];
    for (unsigned i = 0; i < len; ++i)
        L[i][i] = 1;                  /* a single bit is a palindrome of length 1 */

    for (unsigned sublen = 2; sublen <= len; ++sublen) {
        for (unsigned i = 0; i + sublen <= len; ++i) {
            unsigned j = i + sublen - 1;
            uint8_t val;
            if (bits[i] == bits[j]) {
                uint8_t inner = (sublen <= 2) ? 0 : L[i + 1][j - 1];
                val = inner + 2;
            } else {
                uint8_t a = L[i + 1][j];
                uint8_t b = L[i][j - 1];
                val = (a > b) ? a : b;
            }
            L[i][j] = val;
        }
    }

    /* Longest palindrome subsequence that starts with the leading 1
       (bit 0).  It must end at some position j where bits[j]==1. */
    unsigned best_len = 1;            /* subsequence consisting of the first bit only */
    for (unsigned j = 1; j < len; ++j) {
        if (bits[j] != 1) continue;
        unsigned inner = 0;
        if (j >= 2) inner = L[1][j - 1];
        unsigned cand = inner + 2;
        if (cand > best_len)
            best_len = cand;
    }

    unsigned deletions = len - best_len;
    return deletions == 5;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;           /* cached sequence terms          */
    static size_t cache_cap = 0;          /* current allocation capacity    */
    static size_t cache_len = 0;          /* number of terms stored         */
    static seq_t next_val = 1;            /* next value to test             */

    while (cache_len < (size_t)n) {
        if (has_delete_five(next_val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {                     /* allocation failed */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_val;
        }
        ++next_val;
    }

    return cache[(size_t)n - 1];
}
