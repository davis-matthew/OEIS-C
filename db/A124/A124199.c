/*
 * A124199  Primes of the form k(k+1)/2-2 (i.e., two less than triangular
 *          numbers).
 * https://oeis.org/A124199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 92, u32 n <= 141, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A124199
    a(n) = the n‑th prime p for which p = k(k+1)/2 – 2  (k ≥ 2),
    equivalently  8p + 17 is a perfect square.  Off‑by‑one offset: a(1)=13.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* cache of already found primes */
    static seq_t *cache      = NULL;   /* array of primes of the form k(k+1)/2‑2 */
    static size_t cache_len  = 0;      /* number of primes cached          */
    static size_t cache_cap  = 0;      /* current capacity of cache        */
    static idx_t  next_k     = 5;       /* next k to test (k=5 gives 13)    */

    if (n == 0)                 /* offset is 1, no entry for n=0 */
        return 0;

    /* Ensure we have at least n entries in the cache */
    while (cache_len < n) {
        /* Compute candidate: t = k(k+1)/2 – 2  */
        seq_t t = ((seq_t)next_k * (next_k + 1)) / 2;
        t -= 2;

        if (t >= 2 && is_prime_big(t)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)          /* allocation failure */
                    return 0;
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = t;
        }
        next_k++;
    }

    return cache[n - 1];
}
