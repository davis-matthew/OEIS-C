/*
 * A050422  Numbers for which in base 2 the least number of digits that can be
 *          removed to leave a palindrome (possibly beginning with 0) is 3.
 * https://oeis.org/A050422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Property: minimal number of digit deletions needed to leave a binary
    palindrome (allowing leading zeroes) is exactly 3.
    The function computes a(n) by brute‑forcing increasing numbers, caching
    the ones that satisfy the property.  DP of size 64×64 is used to
    compute the longest palindromic subsequence – O(1) per candidate.  */

#include <stdlib.h>   /* malloc, realloc, NULL */
#include <string.h>   /* memset */
#include "../utils/mem.h"

/* test whether ``x`` satisfies the property */
static int satisfies_property(seq_t x)
{
    /* compute bit length (at least 1) */
    idx_t len = 0;
    seq_t y = x;
    while (y) { y >>= 1; ++len; }
    if (len == 0) len = 1;            /* x == 0 */

    /* store bits from most significant to least significant */
    int bits[64];                     /* seq_t is at most 128 bits; 64 enough for 64‑bit seq_t */
    y = x;
    for (idx_t i = 0; i < len; ++i) {
        bits[len-1-i] = (int)(y & 1);
        y >>= 1;
    }

    /* DP for longest palindromic subsequence (O(len²)) */
    int dp[64][64] = {0};
    for (int i = (int)len - 1; i >= 0; --i) {
        dp[i][i] = 1;
        for (int j = i + 1; j < (int)len; ++j) {
            if (bits[i] == bits[j])
                dp[i][j] = dp[i + 1][j - 1] + 2;
            else {
                int a = dp[i + 1][j];
                int b = dp[i][j - 1];
                dp[i][j] = a > b ? a : b;
            }
        }
    }

    int lps = dp[0][(int)len - 1];
    return ( (int)len - lps ) == 3;
}

static seq_t *cache = NULL;
static size_t cache_used  = 0;   /* number of elements stored */
static size_t cache_cap   = 0;   /* allocated capacity */
static seq_t next_candidate = 1; /* current integer to test */

/* enlarge the cache when necessary */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t newsize = new_cap * sizeof(seq_t);
    seq_t *tmp;
    if (cache == NULL)
        tmp = mem_new(seq_t, (newsize) / sizeof(seq_t));
    else
        tmp = mem_resize(seq_t, cache, (newsize) / sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* public sequence routine */
seq_t a(idx_t n)
{
    /* grow the cache as long as needed */
    while (cache_used < n) {
        if (satisfies_property(next_candidate)) {
            if (cache_used + 1 > cache_cap)
                ensure_cache_capacity((cache_cap == 0) ? 16 : cache_cap * 2);
            cache[cache_used++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
