/*
 * A225739  Palindromic squares whose sum of digits is also a palindromic
 *          square.
 * https://oeis.org/A225739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th palindromic square whose digit sum is also a *
 *            palindromic square. The sequence consists of the squares * *
 *            (10^k + 1)^2 for k = 0,1,2,... * (100^k + 10^k + 1)^2 for k =
 *            1,2,3,... * * combined with the special initial terms 1^2=1 and
 *            3^2=9. The function * merges the two families...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow10.h"

/* Static cache of computed terms.  The array is grown lazily on demand. */
static seq_t *cache = NULL;
static size_t  cache_len   = 0;
static size_t  cache_cap   = 0;
static idx_t   next_a_idx  = 1;   /* next k for (10^k+1)^2   (k=1 yields 121) */
static idx_t   next_b_idx  = 1;   /* next k for (100^k+10^k+1)^2 */

/* Append value to cache, growing it if necessary. */
static void push_cache(seq_t value)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = value;
}

seq_t a(idx_t n)
{
    /* Initialise cache with the first three terms. */
    if (cache == NULL) {
        cache_cap   = 64;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len   = 0;
        push_cache((seq_t)1);   /* 1^2 = 1 */
        push_cache((seq_t)4);   /* (10^0+1)^2 = 4 */
        push_cache((seq_t)9);   /* 3^2  = 9 */
        /* next indices already set to 1 */
    }

    /* Produce new terms until we have at least n elements. */
    while (cache_len < (size_t)n) {
        /* Compute the next candidate from each family. */
        seq_t baseA = pow10(next_a_idx);
        baseA += (seq_t)1;
        seq_t valA = baseA * baseA;

        seq_t pow10_2k = pow10(2 * next_b_idx);
        seq_t pow10_k  = pow10(next_b_idx);
        seq_t baseB    = pow10_2k;
        baseB += pow10_k;
        baseB += (seq_t)1;
        seq_t valB = baseB * baseB;

        /* Select the smaller value (or either if equal) and advance its index. */
        if (valA <= valB) {
            push_cache(valA);
            ++next_a_idx;
        } else {
            push_cache(valB);
            ++next_b_idx;
        }
    }

    return cache[n - 1];
}
