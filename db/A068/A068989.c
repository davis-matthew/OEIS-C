/*
 * A068989  Squares which when reversed are primes (ignore leading zeros).
 * https://oeis.org/A068989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 16, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th square whose decimal reversal is a prime (ignoring leading zeros) */
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t length = 0, capacity = 0;
    static idx_t last_root = 0;

    if (n == 0) return 0; /* Undefined for n < 1 */

    while (length < (size_t)n) {
        ++last_root;
        seq_t sq = (seq_t)last_root * (seq_t)last_root;

        /* Reverse decimal digits of sq */
        seq_t rev = 0;
        seq_t temp = sq;
        while (temp > 0) {
            rev = rev * 10 + (temp % 10);
            temp /= 10;
        }

        if (is_prime_big(rev)) {
            if (length == capacity) {
                capacity = capacity ? capacity * 2 : 16;
                cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            }
            cache[length++] = sq;
        }
    }

    return cache[n - 1];
}
