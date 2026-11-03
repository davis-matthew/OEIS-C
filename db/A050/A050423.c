/*
 * A050423  Numbers for which in base 2 the least number of digits that can be
 *          removed to leave a palindrome (possibly beginning with 0) is 4.
 * https://oeis.org/A050423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A050423: Numbers for which in base 2 the least number of digits that can be 
 * removed to leave a palindrome (possibly beginning with 0) is 4. 
 * 
 * The implementation searches forward for numbers with exactly 4 deletions
 * needed to make their binary representation a palindrome.  The results
 * are cached so the function works in any call order.  The algorithm
 * uses a straightforward O(L^2) dynamic‑programming routine for each
 * number where L is its bit length (≤ 64 because idx_t is 64‑bit).
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute the number of bits in an idx_t value (bit length). */
static unsigned bitlen_idx_t(idx_t v)
{
    unsigned len = 0;
    do {
        ++len;
        v >>= 1;
    } while (v);
    return len;
}

/* Return the minimal number of deletions required to make the binary
   representation of `val` a palindrome. */
static int lps_min_del(idx_t val)
{
    unsigned L = bitlen_idx_t(val);
    if (L <= 1) return 0;               /* already a palindrome */

    /* Extract bits: bits[0] is MSB, bits[L-1] is LSB. */
    uint8_t bits[128];
    idx_t tmp = val;
    for (unsigned i = 0; i < L; ++i) {
        bits[L - 1 - i] = (uint8_t)(tmp & 1);
        tmp >>= 1;
    }

    /* Longest palindromic subsequence DP. */
    static int dp[128][128];
    for (int i = (int)L - 1; i >= 0; --i) {
        dp[i][i] = 1;
        for (int j = i + 1; j < (int)L; ++j) {
            if (bits[i] == bits[j]) {
                dp[i][j] = 2 + ((i + 1 <= j - 1) ? dp[i + 1][j - 1] : 0);
            } else {
                int a = dp[i + 1][j];
                int b = dp[i][j - 1];
                dp[i][j] = (a > b) ? a : b;
            }
        }
    }

    int deletions = (int)L - dp[0][L - 1];
    return deletions;
}

/* Cached list of found terms. */
static seq_t *cache   = NULL;
static size_t cacheSize = 0;   /* number of terms stored */
static size_t cacheCap  = 0;   /* allocated capacity */
static idx_t lastChecked = 0;  /* largest integer examined so far */

/* Ensure that the cache has capacity for at least `newCap` elements. */
static void grow_cache(size_t newCap)
{
    if (newCap <= cacheCap) return;
    size_t newSize = newCap > 16 ? newCap : 16;
    if (cache == NULL) {
        cache = mem_new(seq_t, newSize);
    } else {
        seq_t *tmp = mem_resize(seq_t, cache, newSize);
        cache = tmp;
    }
    cacheCap = newSize;
}

/* Return the n-th term of A050423 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    /* Generate terms until we have at least n entries. */
    while (cacheSize < n) {
        idx_t cur = lastChecked + 1;
        while (true) {
            if (lps_min_del(cur) == 4) {
                grow_cache(cacheSize + 1);
                cache[cacheSize++] = (seq_t)cur;
                lastChecked = cur;
                break;
            }
            ++cur;
        }
    }

    return cache[n - 1];
}
