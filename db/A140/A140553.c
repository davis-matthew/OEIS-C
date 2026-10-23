/*
 * A140553  Primes p such that neither p - 210 nor p + 210 is prime.
 * https://oeis.org/A140553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute force: find primes p such that neither p‑210 nor p+210 is prime.
   Memoized to handle random access.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;
static idx_t current = 2;
#else
static seq_t *cache = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;
static idx_t   current = 2;
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

#ifdef __cplusplus
    while (cache.size() < n) {
        if (is_prime_big((seq_t)current)) {
            bool ok = true;
            if (current > 210 && is_prime_big((seq_t)(current - 210)))
                ok = false;
            if (is_prime_big((seq_t)(current + 210)))
                ok = false;
            if (ok) cache.push_back((seq_t)current);
        }
        ++current;
    }
    return cache[n - 1];
#else
    while (cache_len < n) {
        if (is_prime_big((seq_t)current)) {
            bool ok = true;
            if (current > 210 && is_prime_big((seq_t)(current - 210)))
                ok = false;
            if (is_prime_big((seq_t)(current + 210)))
                ok = false;
            if (ok) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;   /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)current;
            }
        }
        ++current;
    }
    return cache[n - 1];
#endif
}
