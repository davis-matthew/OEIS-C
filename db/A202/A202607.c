/*
 * A202607  Numbers n such that 6n and sigma(6n) are both a twin prime
 *          average.
 * https://oeis.org/A202607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) is the nth positive integer n such that * 6n and σ(6n) are
 *            both twin‑prime averages * (i.e. 6n−1, 6n+1, σ(6n)−1 and σ(6n)+1
 *            are all prime)
 * Range:     u8 n <= 2, u16 n <= 27, u32 n <= 36, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /*  Cache the sequence values computed so far.  */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;     /* number of terms stored   */
    static size_t cache_cap  = 0;     /* current capacity of cache*/
    static idx_t  tested_n   = 1;     /* next n to test           */

    /*  The requested term may not yet be in the cache.  */
    while (cache_len < n) {

        /*  Search for the next valid n.  */
        while (1) {
            idx_t m = 6ULL * tested_n;   /* m = 6·n */

            /*  Quick small‑prime test on the twin‑prime pair around 6n.  */
            if (is_prime_small(m - 1) && is_prime_small(m + 1)) {

                /*  Compute σ(6n).  */
                seq_t sigma = sigma_of(m);

                /*  Quick big‑prime test on the twin‑prime pair around σ(6n).  */
                if (is_prime_big(sigma - 1) && is_prime_big(sigma + 1)) {
                    /*  Found a term: store it.  */
                    if (cache_len == cache_cap) {
                        cache_cap = cache_cap ? 2 * cache_cap : 8;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_len++] = (seq_t)tested_n;
                }
            }

            /*  Move to the next candidate regardless of outcome.  */
            tested_n++;

            /*  If we have enough terms, exit the inner loop.  */
            if (cache_len >= n) break;
        }
    }

    /*  Return the requested term.  */
    return cache[n - 1];
}
