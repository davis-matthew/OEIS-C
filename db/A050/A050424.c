/*
 * A050424  Numbers for which in base 2 the least number of digits that can be
 *          removed to leave a palindrome (possibly beginning with 0) is 5.
 * https://oeis.org/A050424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th term of OEIS A050424.
   The term is the n‑th integer whose binary representation can be
   turned into a palindrome by deleting exactly 5 bits (the smallest
   possible deletions). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Return the minimal number of deletions needed in the binary
   representation of x to obtain a palindrome.  The algorithm
   uses a dynamic‑programming table of size len^2 where len is the
   number of bits of x.  For the purposes of this sequence we only
   need to know whether the result equals 5. */
static int min_del_to_pal(seq_t x)
{
    size_t len = (size_t)bitlen_seq_t(x);
    if (len <= 1)
        return 0;

    /* Allocate a buffer for the bits of x, most significant first. */
    unsigned char *bits = (unsigned char *)malloc(len);
    if (!bits)
        return 0;                       /* allocation failure */

    seq_t tmp = x;
    for (int i = (int)len - 1; i >= 0; --i) {
        /* Store the least significant bit.  This works for built‑in
           integer types as well as the arbitrary‑precision class
           because the operators & and == are expected to be
           overloaded. */
        bits[i] = (unsigned char)((tmp & 1) != 0);
        tmp >>= 1;
    }

    /* DP for minimal deletions to palindrome.  dp[j] holds dp[i+1][j]
       for the current i. */
    int *dp = (int *)calloc(len, sizeof(int));
    if (!dp) {
        free(bits);
        return 0;
    }

    for (int i = (int)len - 1; i >= 0; --i) {
        int prev = 0;   /* dp[i+1][i] == 0 */
        for (int j = i + 1; j < (int)len; ++j) {
            int cur = dp[j];
            if (bits[i] == bits[j]) {
                dp[j] = prev;
            } else {
                int left  = dp[j];        /* dp[i+1][j] */
                int right = dp[j - 1];    /* dp[i][j-1] */
                dp[j] = (left < right ? left : right) + 1;
            }
            prev = cur;
        }
    }

    int result = dp[len - 1];
    free(dp);
    free(bits);
    return result;
}

/* Memoised search for the n-th element of the sequence.  The cache
   stores all previously found terms.  We keep a running counter
   `last_tested` to avoid re‑testing numbers when the function
   is called out of order. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_tested = 1;   /* next integer to examine */

    if (n == 0)          /* OEIS offset is 1; behaviour for n==0 is
                            undefined, return 0. */
        return 0;

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        seq_t candidate = (seq_t)last_tested;
        if (min_del_to_pal(candidate) == 5) {
            /* Resize cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache)   /* allocation failure; fallback */
                    return 0;
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        ++last_tested;
    }

    return cache[n - 1];
}
