/*
 * A038509  Composite numbers congruent to +-1 mod 6.
 * https://oeis.org/A038509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   A038509 consists of composite numbers that are ≡ ±1 (mod 6). The
 *            algorithm simply searches upward from 5, skips primes, and
 *            caches the results for fast subsequent look‑ups
 * Range:     u8 n <= 32, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next integer to test; 5 is the smallest candidate that could start the sequence */
    static idx_t candidate = 5;

    if (n == 0) return 0;          /* the sequence is 1‑indexed */

    /* grow the cache array if we need to store more terms */
    if (cache_len < n && cache_cap < n) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* extend the cache until it contains n terms */
    while (cache_len < n) {
        /* find the next composite ≡ ±1 mod 6 */
        while (1) {
            if ((candidate % 6 == 1 || candidate % 6 == 5) &&
                !is_prime(candidate)) {
                cache[cache_len++] = (seq_t)candidate;
                candidate++;        /* move to the next number for future searches */
                break;
            }
            candidate++;
        }
    }

    return cache[n - 1];
}
