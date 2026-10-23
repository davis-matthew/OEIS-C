/*
 * A027758  Primes of the form k^2 + k + 9.
 * https://oeis.org/A027758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form k^2 + k + 9, k >= 0
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 28, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Internal cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static idx_t next_k = 0;   /* next k to test */

    if (n == 0) return 0; /* safety, though offset is 1 */

    while (cache_cnt < n) {
        /* Compute f(k) = k^2 + k + 9 in seq_t */
        seq_t val = (seq_t)next_k * (seq_t)next_k + (seq_t)next_k + 9;

        if (is_prime_big(val)) {
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = val;
        }
        next_k++;
    }
    return cache[n - 1];
}
