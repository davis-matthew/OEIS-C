/*
 * A050421  Numbers for which in base 2 the least number of digits that can be
 *          removed to leave a palindrome (possibly beginning with 0) is 2.
 * https://oeis.org/A050421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer whose binary representation * has
 *            minimal number of deletions required to become a palindrome *
 *            equal to exactly 2 (allowing leading zeros in the resulting
 *            palindrome)
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   extract_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/extract_bits.h"

/* Maximum number of bits we expect seq_t to support in the tests.
 * This value can be increased if larger numbers are required. */
#define MAX_BITS 128

/* Longest palindromic subsequence length of bits[0..len-1].
 * Uses O(len^2) DP; len is at most MAX_BITS. */
static int
lps_length(const uint8_t *bits, size_t len)
{
    if (len == 0)
        return 0;
    static int dp[MAX_BITS][MAX_BITS];
    for (size_t i = 0; i < len; ++i)
        dp[i][i] = 1;
    for (size_t l = 2; l <= len; ++l) {
        for (size_t i = 0; i + l - 1 < len; ++i) {
            size_t j = i + l - 1;
            if (bits[i] == bits[j]) {
                dp[i][j] = 2 + (i + 1 <= j - 1 ? dp[i + 1][j - 1] : 0);
            } else {
                int v1 = dp[i][j - 1];
                int v2 = dp[i + 1][j];
                dp[i][j] = v1 > v2 ? v1 : v2;
            }
        }
    }
    return dp[0][len - 1];
}

/* Test if the integer x satisfies the sequence definition. */
static bool
satisfies_property(seq_t x)
{
    uint8_t bits[MAX_BITS];
    size_t len = extract_bits(x, bits);
    if (len < 3)
        return false;                    /* cannot need 2 deletions */
    int lps = lps_length(bits, len);
    if ((int)len - lps != 2)
        return false;                    /* minimal deletions not 2 */
    /* Since min deletions == 2, one deletion cannot make a palindrome */
    return true;
}

/* Compute the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;

    /* Grow cache when necessary. */
    while (cache_size < n) {
        if (cache_cap == 0) {
            cache_cap = 256;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_size == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        /* Find next term that satisfies the property */
        static seq_t cand = 1;
        while (!satisfies_property(cand)) {
            cand += 1;
        }
        cache[cache_size++] = cand;
        cand += 1;
    }
    return cache[n - 1];
}
