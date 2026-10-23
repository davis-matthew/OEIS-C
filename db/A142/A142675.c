/*
 * A142675  Primes congruent to 16 mod 57.
 * https://oeis.org/A142675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime p such that p ≡ 16 (mod 57)
 * Range:     u8 n <= 1, u16 n <= 177, u32 n <= 101, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates candidates 57k+16 and tests them for primality.
   It keeps a static cache of discovered primes so subsequent calls are fast. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;   /* stored primes  */
    static size_t cap       = 0;      /* capacity          */
    static size_t sz        = 0;      /* number of primes  */
    static seq_t last_cand  = 0;      /* last tested candidate */

    /* initialise cache with the first known prime 73 */
    if (sz == 0) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;               /* unlikely failure */
        cache[0] = (seq_t)73;               /* first prime ≡ 16 (mod 57) */
        sz = 1;
        last_cand = (seq_t)73;
    }

    /* generate further primes until reaching the requested index */
    while (sz < n) {
        last_cand += (seq_t)57;                 /* next number ≡ 16 (mod 57) */
        if (is_prime_big(last_cand)) {           /* primality test */
            if (sz == cap) {                    /* grow cache if needed */
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) return 0;              /* allocation failure */
                cache = tmp;
            }
            cache[sz++] = last_cand;
        }
    }

    return cache[n - 1];
}
