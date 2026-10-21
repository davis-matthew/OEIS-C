/*
 * A088619  Numbers k such that the concatenation of 1, k and k^2 is prime.
 * https://oeis.org/A088619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1, u32 n <= 1, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digits, is_prime_big, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: returns the n‑th integer k such that the concatenation
   of the decimal representations of 1, k and k² is a prime number. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of cached array   */
    static size_t cache_len = 0;   /* number of terms cached    */
    static idx_t tested_k = 0;     /* last k that has been tested */

    if (n == 0) return 0;  /* offset is 1, but guard against 0 */

    while (cache_len < n) {
        ++tested_k;

        /* k² within 64‑bit, safe for digits() */
        idx_t k2_64 = tested_k * tested_k;

        idx_t dk      = digits(tested_k);
        idx_t dksq    = digits(k2_64);

        seq_t val = seq_mul((seq_t)1, pow10(dk + dksq));
        seq_t part = seq_mul((seq_t)tested_k, pow10(dksq));
        val += part;
        val += (seq_t)k2_64;

        if (is_prime_big(val)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* memory exhausted, abort */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = tested_k;
        }
    }
    return cache[n - 1];
}
