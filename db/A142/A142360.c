/*
 * A142360  Primes congruent to 9 mod 47.
 * https://oeis.org/A142360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 9 (mod 47)
 * Range:     u8 n <= 2, u16 n <= 139, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/seq_realloc.h"

static seq_t *prime_cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static uint64_t last_k = 0;  /* the last k processed for 47*k+9 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (cache_len < n) {
        last_k++;
        uint64_t candidate = 47ULL * last_k + 9ULL;
        if (is_prime_small(candidate)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = seq_realloc(prime_cache, cache_cap, new_cap);
                if (!tmp) abort();
                prime_cache = tmp;
                cache_cap = new_cap;
            }
            prime_cache[cache_len++] = (seq_t)candidate;
        }
    }
    return prime_cache[n - 1];
}
