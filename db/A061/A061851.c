/*
 * A061851  Digital representation of n contains only 1's and 0's, is
 *          palindromic and contains no singleton 1's or 0's.
 * https://oeis.org/A061851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 19, u64 n <= 231,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 42 terms through n = 9685 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Constructive enumeration of palindromic decimal strings
   containing only 0 and 1, with no singleton 0's or 1's.
   Terms are generated in order of increasing length, and for
   equal length in increasing numeric value.  This order
   reproduces OEIS A061851. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper to test run‑length condition */
static bool valid_runs(const unsigned char *dig, idx_t len)
{
    idx_t i = 0;
    while (i < len) {
        unsigned char cur = dig[i];
        idx_t run = 1;
        while (i + 1 < len && dig[i + 1] == cur) {
            ++i;
            ++run;
        }
        if (run < 2) return false;
        ++i;
    }
    return true;
}

/* Generate all admissible palindromes of length 'len'
   and append them to the global cache. */
static void generate_length(idx_t len,
                            seq_t **cache, idx_t *csize, idx_t *ccap)
{
    /* Number of digits in the left half (including centre for odd len) */
    const idx_t khalf = (len + 1) / 2;
    /* Number of free bits after the leading 1 */
    const idx_t freebits = khalf - 1;

    /* If no free bits, only one candidate (all leading 1s) */
    const idx_t maxmask = (freebits == 0) ? 0 : ((1ULL << freebits) - 1);

    /* Ensure the cache can hold some extra elements */
    if (*ccap == 0) {
        *ccap = 256;
        *cache = mem_new(seq_t, (*ccap * sizeof(seq_t)) / sizeof(seq_t));
    }

    unsigned char left[64];      /* left half digits, 0 or 1 */
    unsigned char digits[128];   /* full palindrome digits */

    for (idx_t mask = 0; mask <= maxmask; ++mask) {
        /* Build left half: left[0] is the leading 1 */
        left[0] = 1;
        for (idx_t i = 1; i < khalf; ++i) {
            /* Use bits with the most significant free bit for the
               earliest position after the leading 1 (lexicographic order). */
            left[i] = (mask >> (freebits - i)) & 1U;
        }

        /* Build full palindrome */
        for (idx_t i = 0; i < len; ++i) {
            if (i < khalf)
                digits[i] = left[i];
            else
                digits[i] = left[len - 1 - i];
        }

        if (!valid_runs(digits, len))
            continue;

        /* Convert digit array to seq_t value */
        seq_t val = 0;
        for (idx_t i = 0; i < len; ++i) {
            val = val * 10 + (seq_t)digits[i];
        }

        /* Append to cache */
        if (*csize == *ccap) {
            *ccap <<= 1;
            *cache = mem_resize(seq_t, *cache, (*ccap * sizeof(seq_t)) / sizeof(seq_t));
        }
        (*cache)[(*csize)++] = val;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    static idx_t next_len = 2;          /* first admissible length */

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        generate_length(next_len, &cache, &cache_size, &cache_cap);
        ++next_len;
    }

    return cache[n - 1];
}
