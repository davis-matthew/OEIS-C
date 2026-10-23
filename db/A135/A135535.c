/*
 * A135535  Primes of the form 4^k - 3.
 * https://oeis.org/A135535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^k - 3 where k is the n‑th exponent such that 4^k-3 is
 *            prime
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 5, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_big, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* Cached terms */
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    /* Next exponent to test (k) */
    static idx_t next_k = 2;

    /* Lazy initialization of cache */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (len < n) {
        /* 4^k */
        seq_t val = seq_pow((seq_t)4, next_k);
        /* 4^k - 3 */
        val -= (seq_t)3;

        if (is_prime_big(val)) {
            /* Grow cache if necessary */
            if (len == cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = val;
        }

        ++next_k;
    }

    return cache[n - 1];
}
