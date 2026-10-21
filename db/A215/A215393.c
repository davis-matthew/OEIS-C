/*
 * A215393  Primes congruent to {1, 2} mod 19.
 * https://oeis.org/A215393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   set of primes congruent to 1 or 2 modulo 19, in ascending order.
 *            a(1)=2, a(2)=59, a(3)=97,
 * Range:     u8 n <= 7, u16 n <= 724, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* static cache of primes we have discovered */
static seq_t *prime_cache     = NULL;
static size_t prime_cache_size = 0;
static size_t prime_cache_cap  = 0;
static uint64_t next_candidate = 2;

/* ensure the cache has room for at least 'needed' entries */
static void ensure_prime_cache_capacity(size_t needed)
{
    if (needed <= prime_cache_cap) return;
    size_t new_cap = needed * 2;
    if (new_cap < 8) new_cap = 8;
    seq_t *ptr = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    prime_cache = ptr;
    prime_cache_cap = new_cap;
}

/* external linkage, as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; guard against n==0 */

    /* grow the cache until we have at least n entries */
    while (prime_cache_size < n) {
        uint64_t x = next_candidate++;
        if ((x % 19ULL) == 1ULL || (x % 19ULL) == 2ULL) {
            if (is_prime_small(x)) {
                ensure_prime_cache_capacity(prime_cache_size + 1);
                prime_cache[prime_cache_size++] = (seq_t)x;
            }
        }
    }
    return prime_cache[n - 1];
}
