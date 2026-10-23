/*
 * A127072  Numbers k that divide 3^k - 2^k - 1.
 * https://oeis.org/A127072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 6623, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th term of OEIS A127072.
   It computes the sequence by testing each integer k  (k >= 1)
   for the divisibility condition k | (3^k - 2^k - 1).
   For the test it uses modular exponentiation mod_pow from the
   provided utilities.  Results are cached so the function runs
   in O(k log k) time for the k needed to reach the n-th term. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

/* Test whether k divides 3^k - 2^k - 1.
   Uses 128-bit arithmetic to avoid overflow of intermediate
   unsigned results.  All intermediate values fit in uint64_t
   because mod_pow returns a value < k. */
static inline bool test_k(idx_t k)
{
    seq_t mod   = (seq_t)k;
    seq_t a = mod_pow(3, k, mod);
    seq_t b = mod_pow(2, k, mod);

    uint64_t a64 = (uint64_t)a;
    uint64_t b64 = (uint64_t)b;
    uint64_t mod64 = (uint64_t)k;

    __uint128_t temp = (__uint128_t)a64 + (__uint128_t)mod64 - (__uint128_t)b64;
    temp %= mod64;
    temp = (temp + mod64 - 1) % mod64;

    return temp == 0;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static size_t sat = 0;        /* number of cached terms */
    static size_t cap  = 0;        /* capacity of cache array */
    static idx_t cur_k = 1;        /* next integer to test */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap  = 128;
        sat  = 0;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend cache until we have at least n terms */
    while (sat < n) {
        if (test_k(cur_k)) {
            if (sat == cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[sat++] = (seq_t)cur_k;
        }
        cur_k++;
    }

    /* return requested term (1‑based index) */
    return cache[n - 1];
}
