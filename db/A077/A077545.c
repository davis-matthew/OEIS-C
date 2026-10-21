/*
 * A077545  Primes of the form floor(k*e).
 * https://oeis.org/A077545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A077545 (primes of the form floor(k*e)).
   Using long double arithmetic to get floor(k*e) for the needed range. */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    /* Cached list of found primes of the form floor(k*e) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 0;       /* next k to test is last_k+1 */

    /* Constant e with long double precision */
    const long double E = 2.7182818284590452353602874713527L;

    /* Extend cache until we have at least n primes */
    while (cache_len < (size_t)n) {
        last_k++;
        /* Compute floor(last_k * e) as an integer */
        unsigned long long val_ul =
            (unsigned long long)floorl((long double)last_k * E);
        seq_t val = (seq_t)val_ul;

        /* Test primality */
        if (is_prime_big(val)) {
            /* Allocate / grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;            /* memory allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }

    return cache[n - 1];   /* indices are 1‑based in OEIS */
}
