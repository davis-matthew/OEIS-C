/*
 * A158314  Consider a twin prime pair (p, q=p+2) such that the number of
 *          prime digits in p is equal to the number of odd digits in q;
 *          sequence lists values of p.
 * https://oeis.org/A158314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   count_prime_digits_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for twin primes (p, p+2) where the number of prime digits in p equals
   the number of odd digits in p+2.  Cache results to answer queries in any order. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/count_prime_digits_uint64.h"

static inline int count_odd_digits_uint64(uint64_t x)
{
    int cnt = 0;
    while (x) {
        uint8_t d = x % 10;
        if (d == 1 || d == 3 || d == 5 || d == 7 || d == 9) ++cnt;
        x /= 10;
    }
    return cnt;
}

static uint64_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 3;

static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    uint64_t *tmp = (uint64_t*)realloc(cache, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (cache_len < (size_t)n) {
        uint64_t p = next_candidate;
        if (is_prime((idx_t)p) && is_prime((idx_t)(p + 2))) {
            if (count_prime_digits_uint64(p) == count_odd_digits_uint64(p + 2)) {
                if (cache_len == cache_cap) grow_cache();
                cache[cache_len++] = p;
            }
        }
        next_candidate += 2;
    }
    return (seq_t)cache[n - 1];
}
