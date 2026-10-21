/*
 * A060231  Smaller of twin primes whose middle term is a multiple of
 *          A002110(5)=2310.
 * https://oeis.org/A060231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2310·k − 1 for the n‑th integer k such that 2310·k − 1
 *            and 2310·k + 1 are both prime (the twin primes whose middle term
 *            is a multiple of 2310)
 * Range:     u8 none, u16 n <= 6, u32 n <= 7, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static idx_t   cache_size = 0;   /* number of terms already found */
    static idx_t   next_k     = 1;   /* next k to be tested */
    static size_t  cache_cap  = 0;   /* array capacity (in terms) */
    static seq_t * cache      = NULL;/* array of found terms */

    /* Expand the cache until it contains at least n terms. */
    while (cache_size < n) {
        seq_t base = (seq_t)2310 * next_k;
        seq_t p    = base - 1;  /* 2310·k − 1 */
        seq_t q    = base + 1;  /* 2310·k + 1 = p + 2 */

        if (is_prime_big(p) && is_prime_big(q)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0;           /* allocation failure */
                cache = tmp;
            }
            cache[cache_size++] = p;
        }
        ++next_k;
    }

    return cache[n - 1]; /* zero‑based array index */
}
