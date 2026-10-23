/*
 * A166561  Primes p such that sum of digits + 1 is prime.
 * https://oeis.org/A166561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime p such that digit_sum(p)+1 is prime
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t capacity   = 0;
    static size_t cache_size = 0;   /* number of terms stored   */
    static idx_t  last_checked = 1; /* last integer tested      */

    if (n == 0) return 0;          /* offset starts at 1       */

    while (cache_size < (size_t)n) {
        /* find the next prime */
        do {
            ++last_checked;
        } while (!is_prime(last_checked));

        seq_t  candidate = last_checked;          /* prime value as seq_t */
        seq_t  ds        = digit_sum(candidate);   /* sum of digits */
        seq_t  ds_plus1  = ds + (seq_t)1;          /* sum+1 */

        if (is_prime((idx_t)ds_plus1)) {
            /* ensure enough capacity for the new value */
            if (cache_size >= capacity) {
                size_t new_cap = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                capacity = new_cap;
            }
            cache[cache_size++] = candidate;      /* store term */
        }
    }

    return cache[n - 1];
}
