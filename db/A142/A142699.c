/*
 * A142699  Primes congruent to 55 mod 57.
 * https://oeis.org/A142699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 182, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching primes ≡ 55 (mod 57) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *prime_mod55_cache = NULL;
static size_t prime_mod55_count = 0;
static size_t prime_mod55_cap   = 0;
static uint64_t next_k = 4;          /* 57*4+55 = 283  */
static bool initialized = false;

/* ensure the cache can hold at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= prime_mod55_cap)
        return;
    size_t new_cap = prime_mod55_cap ? prime_mod55_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prime_mod55_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    prime_mod55_cache = tmp;
    prime_mod55_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* offset is 1; undefined for 0 */
    if (!initialized) {
        initialized = true;
        grow_cache(16);
    }
    while (prime_mod55_count < (size_t)n) {
        uint64_t candidate = 57 * next_k + 55;
        if (is_prime((idx_t)candidate))
            prime_mod55_cache[prime_mod55_count++] = (seq_t)candidate;
        ++next_k;
    }
    return prime_mod55_cache[n - 1];
}
