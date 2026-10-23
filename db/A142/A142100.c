/*
 * A142100  Duplicate of A093838.
 * https://oeis.org/A142100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n-th prime ≡ 1 (mod 36); i.e. primes of the form
 *            36k+1
 * Range:     i8 n <= 3, i16 n <= 16, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"   /* declares is_prime_small(seq_t) */

seq_t a(idx_t n)
{
    static bool init = false;
    static idx_t candidate = 37;        /* first prime of the form 36k+1 */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    if (n == 0) return 0;               /* undefined for n < 1, avoid UB */

    if (!init) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;           /* allocation failed */
        cache_len = 0;
        init = true;
    }

    while (cache_len < n) {
        seq_t val = (seq_t)candidate;
        if (is_prime_small(val)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                if (!tmp) return 0;     /* allocation failed */
                cache = tmp;
            }
            cache[cache_len++] = val;
        }
        /* move to the next candidate of the form 36k+1 */
        candidate += 36;
    }

    return cache[n - 1];
}
