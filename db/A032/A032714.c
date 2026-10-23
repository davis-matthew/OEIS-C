/*
 * A032714  Numbers k such that k prefixed by '6' and followed by '3' is
 *          prime.
 * https://oeis.org/A032714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 31, u32 n <= 99, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_count, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the decimal number 6<k>3 is prime. */

#include <stdlib.h>
#include "../utils/digit_count.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* static cache of found k values */
    static seq_t *cache = NULL;
    static size_t size = 0;
    static size_t cap = 0;

    if (n == 0) return 0; /* undefined for n < 1, but safe */

    /* return cached value if already computed */
    if (size >= (size_t)n) return cache[n - 1];

    /* start searching from the next integer after the last cached k */
    idx_t start = size ? (idx_t)(cache[size - 1]) + 1 : 1;

    for (idx_t k = start; ; ++k) {
        /* build the number 6<k>3 */
        idx_t d = digit_count((uint64_t)k);            /* decimal digits of k */
        seq_t num = pow10((idx_t)(d + 1)) * (seq_t)6
                    + (seq_t)k * (seq_t)10 + (seq_t)3;

        if (is_prime_big(num)) {
            /* append k to cache */
            if (size == cap) {
                size_t new_cap = cap ? cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[size++] = (seq_t)k;

            if (size >= (size_t)n) return cache[n - 1];
        }
    }
}
