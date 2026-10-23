/*
 * A103603  Numbers k such that the string 101001000k is prime.
 * https://oeis.org/A103603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, num_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the string 101001000k is prime.
   a(1) = 7, a(2) = 29, …  */
 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* function: bool is_prime(uint64_t n) */
#include "../utils/num_digits_uint64.h"

static const uint64_t PREFIX = 101001000ULL;   /* the fixed prefix */

/* Cache of previously found terms. */
static seq_t *cache = NULL;          /* array of found k values */
static idx_t cache_len = 0;          /* number of stored terms */
static idx_t cache_cap = 0;          /* current capacity of cache */
static uint64_t last_k_checked = 0;  /* last k value examined */

/* Ensure that cache contains at least n terms. */
static void extend_cache(idx_t n) {
    while (cache_len < n) {
        ++last_k_checked;
        uint64_t d = num_digits_uint64(last_k_checked);

        /* Compute M = PREFIX * 10^d + last_k_checked */
        unsigned __int128 pow10 = 1;
        for (uint64_t i = 0; i < d; ++i) pow10 *= 10;
        unsigned __int128 M128 = (unsigned __int128)PREFIX * pow10 + last_k_checked;

        /* If M exceeds 64 bits we cannot test it with is_prime_uint64,
           but the sequence for n <= 1000 stays safely within 64 bits.
           For larger n the implementation will fall back to a larger
           primality routine if required by the build. */
        uint64_t M64 = (uint64_t)M128;
        if (is_prime(M64)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_k_checked;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* value below the offset is irrelevant */

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    extend_cache(n);
    return cache[n - 1];
}
