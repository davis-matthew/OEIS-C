/*
 * A066074  Primes arising in A066073.
 * https://oeis.org/A066074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime appearing in the sequence of all σ(x)-1
 *            with x composite (x>1). The terms are listed in order of
 *            increasing x, and duplicates are kept. Order of appearance: for
 *            each integer x starting at 2, if x is composite, let p = σ(x)-1.
 *            If p is prime, record p. The first such prime is...
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Memoization of the sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements already stored */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static idx_t next_x = 2;        /* next integer to test */

    if (n == 0) return 0;          /* undefined for offset 0, return 0 */

    /* Grow the cache until it contains at least n terms. */
    while (cache_size < (size_t)n) {
        /* Find the next composite x with σ(x)-1 prime. */
        for (;;) {
            /* Skip primes.  is_prime works on idx_t. */
            if (!is_prime(next_x)) {
                seq_t sigma = sigma_of(next_x);
                seq_t p = sigma - (seq_t)1;
                if (isPrimeGeneric(p)) {
                    /* Append p to the cache. */
                    if (cache_size == cache_cap) {
                        cache_cap = cache_cap ? cache_cap * 2 : 16;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_size++] = p;
                    next_x++;  /* advance to next x for future calls */
                    break;
                }
            }
            next_x++;  /* try the next integer */
        }
    }

    return cache[n - 1];
}
