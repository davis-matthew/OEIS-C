/*
 * A014798  Squares of even square pyramidal numbers.
 * https://oeis.org/A014798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A015222(n+1)^2 (square of the (n+1)th even
 *            square‑pyramidal number)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 27, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache      = NULL;   /* stored squares of even square pyramidal numbers */
    static size_t cache_len  = 0;      /* number of squares currently stored */
    static size_t cache_cap  = 0;      /* current capacity of cache */
    static idx_t   next_k    = 1;       /* next n to examine for evenness */
    static seq_t  current_sum = 0;     /* running sum of squares up to next_k-1 */

    /* helper to grow cache */
    if (cache_len <= n) {
        /* iterate until we have at least n+1 squares stored */
        while (cache_len <= n) {
            /* extend sum to include squares of next_k */
            current_sum += (seq_t)next_k * (seq_t)next_k;

            /* if the sum is even, store its square */
            if ((current_sum & (seq_t)1) == 0) {
                /* ensure enough capacity */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                    if (new_cap <= cache_len) new_cap = cache_len + 1;
                    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache_cap = new_cap;
                }
                cache[cache_len++] = current_sum * current_sum;
            }
            ++next_k;
        }
    }
    return cache[n];
}
