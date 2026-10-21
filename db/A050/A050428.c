/*
 * A050428  Numbers for which in base 2 the least number of digits that can be
 *          removed to leave a base 2 palindromic number (beginning with 1) is
 *          4.
 * https://oeis.org/A050428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) is the nth integer whose binary representation
   needs exactly four deletions to become a binary palindrome that
   starts with 1.  The function finds numbers by brute‑force search,
   computing for each candidate the minimal number of deletions
   (using dynamic programming on the bit string). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/bitlen_seq_t.h"

static seq_t *cache_vals = NULL;   /* memoised terms */
static idx_t cache_len   = 0;       /* number of stored terms */
static idx_t cache_cap   = 0;       /* allocated capacity */
static seq_t next_val  = 1;         /* next integer to test */

/* enlarge cache to at least |cap| entries */
static void ensure_cache_capacity(idx_t cap)
{
    if (cache_cap >= cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < cap) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache_vals,
                                sizeof(seq_t) * (size_t)new_cap);
    if (!p) abort();          /* allocation failure – terminate */
    cache_vals = p;
    cache_cap = new_cap;
}

/* compute minimal deletions needed to make the binary
   representation of x a palindrome that starts with 1.
   Returns a large value if impossible. */
static int minimal_deletions_starting_with_1(seq_t x)
{
    if (x == 0) return 1000;

    idx_t len = bitlen_seq_t(x);
    if (len < 1) return 1000;        /* no bits */

    /* store bits MSB -> LSB */
    unsigned char bits[128];
    seq_t t = x;
    for (idx_t i = 0; i < len; ++i) {
        bits[i] = (unsigned char)(t & 1);
        t >>= 1;
    }

    /* dp[i][j] = length of longest palindromic subsequence
       of bits[i..j] (inclusive).  0 <= i <= j < len. */
    int dp[128][128];
    for (int i = 0; i < (int)len; ++i) dp[i][i] = 1;

    for (idx_t l = 2; l <= len; ++l) {          /* substring length */
        for (int i = 0; i + (int)l - 1 < (int)len; ++i) {
            int j = i + (int)l - 1;
            if (bits[i] == bits[j]) {
                dp[i][j] = 2 + ((i + 1 <= j - 1) ? dp[i + 1][j - 1] : 0);
            } else {
                int a = dp[i + 1][j];
                int b = dp[i][j - 1];
                dp[i][j] = a > b ? a : b;
            }
        }
    }

    /* longest palindrome that starts with a 1 */
    int best = 0;
    for (int i = 0; i < (int)len; ++i) {
        if (bits[i] != 1) continue;
        /* single 1 as palindrome */
        if (best < 1) best = 1;
        for (int j = i + 1; j < (int)len; ++j) {
            if (bits[j] != 1) continue;
            int mid = (i + 1 <= j - 1) ? dp[i + 1][j - 1] : 0;
            int val = 2 + mid;    /* palindrome ends with matching 1 */
            if (val > best) best = val;
        }
    }

    int deletions = (int)len - best;
    return deletions;
}

/* Returns the nth term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    while (cache_len < n) {
        /* find next qualifying integer */
        while (true) {
            int del = minimal_deletions_starting_with_1(next_val);
            if (del == 4) break;
            ++next_val;
        }
        ensure_cache_capacity(cache_len + 1);
        cache_vals[cache_len] = next_val;
        ++cache_len;
        ++next_val;
    }
    return cache_vals[n - 1];
}
