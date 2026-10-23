/*
 * A182331  Primes of the form 6^k + 1.
 * https://oeis.org/A182331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 6^k + 1.
   k runs over 0,1,2,4,8,16,… (powers of two, with 0 allowed).
   The function generates primes in that order, caching results
   for subsequent calls. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    static idx_t exp = 0;              /* current exponent k */
    static const idx_t MAX_EXP = ((idx_t)1 << 28);   /* maximum k to test */

    /* lazy allocation of cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
    }

    /* Generate primes until we have at least n terms or we run out of k. */
    while (cache_len < (size_t)n) {
        if (exp > MAX_EXP) break;     /* no more candidates to test */

        /* candidate = 6^exp + 1 */
        seq_t cand = big_pow((seq_t)6, (seq_t)exp);
        cand += (seq_t)1;

        if (is_prime_big(cand)) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        /* Advance exponent to the next power of two. */
        if (exp == 0)
            exp = 1;
        else
            exp <<= 1;
    }

    if (cache_len < (size_t)n) return (seq_t)0;
    return cache[n - 1];
}
