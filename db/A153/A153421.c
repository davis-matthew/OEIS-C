/*
 * A153421  Primes of the form 2*n^2+11.
 * https://oeis.org/A153421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the nth prime of the form 2*k^2 + 11 (OEIS entry
 *            A153421). The sequence is computed by testing successive values
 *            of k (starting at 0) and keeping the primes found
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */

static seq_t *cache = NULL;          /* array of found primes */
static size_t cache_len = 0;         /* number of elements stored      */
static size_t cache_cap = 0;         /* current array capacity         */
static uint64_t next_k = 0;          /* next k to test  (k starts 0)   */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for 0, but safe      */

    /* expand cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        seq_t k   = (seq_t)next_k;
        seq_t val = (seq_t)2 * k * k + (seq_t)11;

        if (is_prime_big(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failed – return 0 (sequence undefined) */
                    return 0;
                }
                cache    = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
