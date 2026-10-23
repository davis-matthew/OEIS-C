/*
 * A045546  Numbers k such that k^2 + k - 1 is prime.
 * https://oeis.org/A045546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = the n-th positive integer k such that k^2 + k – 1 is
 *            prime
 * Range:     u8 n <= 11, u16 n <= 712, u32 n <= 61, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* array of found k values         */
static size_t cache_len = 0;         /* number of values stored         */
static size_t cache_cap = 0;         /* allocated capacity of cache     */
static idx_t   last_k   = 0;         /* largest k examined so far       */

seq_t a(idx_t n)
{
    /* undefined for n < 1, harmless for the test harness */
    if (n == 0)
        return 0;

    /* grow the cache until it contains at least n terms   */
    while (cache_len < (size_t)n)
    {
        /* Ensure the cache has room */
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failure – abort gracefully */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* next candidate k */
        ++last_k;
        seq_t kk = (seq_t)last_k;                       /* cast to seq_t for arbitrary-precision arithmetic */
        seq_t val = kk * kk + kk - (seq_t)1;            /* compute k^2 + k - 1 as seq_t */

        if (is_prime_big(val))                         /* test primality of the value */
            cache[cache_len++] = kk;                   /* store k if it satisfies the condition */
    }

    /* return the n-th term (1‑indexed) */
    return cache[(size_t)n - 1];
}
