/*
 * A124825  Numbers k such that 10k + 7 and 7k + 10 are primes.
 * https://oeis.org/A124825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 304, u32 n <= 322, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finding the n‑th integer k for which both 10k+7 and 7k+10 are prime. */
/* The sequence is built on demand and cached so that calls in any order
   are fast.  The routine uses is_prime_big which works for arbitrary‑size
   integers and is available through "../utils/is_prime_big.h". */
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

/* static cache for discovered k values */
static seq_t *cache      = NULL;   /* array of found k's (as seq_t)          */
static size_t cache_len  = 0;      /* number of k's stored                   */
static size_t cache_cap  = 0;      /* current capacity of cache             */
static idx_t  next_k     = 1;      /* smallest candidate not yet tested      */

/* a(n) returns the n-th element (n>=1) of A124825 */
seq_t a(idx_t n)
{
    /* Ensure n is positive; behaviour for n<1 is undefined but will
       simply return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Build cache until we have at least n elements. */
    while (cache_len < n) {
        /* Test current candidate k = next_k. */
        seq_t ck1 = 10 * (seq_t)next_k + (seq_t)7;
        seq_t ck2 =  7 * (seq_t)next_k + (seq_t)10;

        if (is_prime_big(ck1) && is_prime_big(ck2)) {
            /* Found a valid k; store it in the cache. */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    /* Return the cached value.  The array is 0‑indexed while OEIS
       indices start at 1. */
    return cache[n - 1];
}
