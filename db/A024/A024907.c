/*
 * A024907  Numbers k such that 9*k - 7 is prime.
 * https://oeis.org/A024907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers k such that 9*k - 7 is prime. Computed by searching k
 *            incrementally and testing primality of 9*k-7
 * Range:     u8 n <= 11, u16 n <= 844, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static idx_t last_k = 0;

    while (cache_len < (size_t)n) {
        ++last_k;
        seq_t candidate = 9 * (seq_t)last_k - 7;
        if (is_prime_big(candidate)) {
            seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failure – return zero for safety, but normally
                   this should not occur for the expected input size. */
                return 0;
            }
            cache = tmp;
            cache[cache_len++] = (seq_t)last_k;
        }
    }
    return cache[(size_t)n - 1];
}
