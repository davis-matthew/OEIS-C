/*
 * A089199  Primes p such that p+1 is divisible by a cube.
 * https://oeis.org/A089199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   A089199 – primes p such that p+1 is divisible by a cube
 * Range:     u8 n <= 16, u16 n <= 1984, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/next_prime.h"
#include "../utils/int_cube_root.h"

/* static storage for small primes up to 2000 (cube root of 8e9) */
static bool  small_primes_init   = false;
static uint32_t small_primes[2000];
static size_t   small_prime_count = 0;

static void
init_small_primes(void)
{
    if (small_primes_init) return;
    const size_t LIMIT = 2000;
    bool marked[LIMIT + 1];
    memset(marked, 0, sizeof(marked));
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!marked[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) {
                marked[j] = true;
            }
        }
    }
    small_prime_count = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!marked[i]) {
            small_primes[small_prime_count++] = (uint32_t)i;
        }
    }
    small_primes_init = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    init_small_primes();

    /* static cache of qualifying primes */
    static seq_t *cache  = NULL;
    static size_t cache_count  = 0;
    static size_t cache_cap    = 0;
    static uint64_t last_checked = 2;   /* start after prime 2 */

    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_count < (size_t)n) {
        /* find the next prime */
        uint64_t p = next_prime(last_checked + 1);
        last_checked = p;

        uint64_t m = p + 1;
        uint64_t r = int_cube_root(m);   /* floor(cuberoot(m)) */

        bool has_cube = false;
        for (size_t i = 0; i < small_prime_count && small_primes[i] <= r; ++i) {
            uint64_t q = small_primes[i];
            uint64_t cube = q * q * q;
            if (cube <= m && (m % cube) == 0) {
                has_cube = true;
                break;
            }
        }
        if (!has_cube) continue;   /* not a qualifying prime */

        if (cache_count >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_count++] = (seq_t)p;
    }

    return cache[n - 1];
}
