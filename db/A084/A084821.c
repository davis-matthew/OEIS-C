/*
 * A084821  Odd numbers m such that sigma(m) > m + phi(m), where sigma=A000203
 *          is the divisor sum and phi=A000010 the totient.
 * https://oeis.org/A084821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd number m such that sigma(m) > m + phi(m)
 * Range:     u8 n <= 12, u16 n <= 3217, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the sequence by testing odd integers sequentially
   and caching results for efficiency. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/int_sqrt_u64.h"

/* prime sieve helpers ---------------------------------------------- */
static uint64_t *prime_arr   = NULL;
static size_t    prime_len   = 0;
static size_t    prime_cap   = 0;
static idx_t     last_prime_done = 1;

static void ensure_primes_upto(idx_t limit) {
    if (limit <= last_prime_done) return;
    idx_t start = last_prime_done + 1;
    for (idx_t i = start; i <= limit; ++i) {
        bool is_p = true;
        uint64_t ii = (uint64_t)i;
        for (size_t j = 0; j < prime_len && (uint64_t)prime_arr[j] * prime_arr[j] <= ii; ++j) {
            if (ii % prime_arr[j] == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (prime_len == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 64;
                prime_arr = (uint64_t*)realloc(prime_arr, prime_cap * sizeof(uint64_t));
            }
            prime_arr[prime_len++] = i;
        }
    }
    last_prime_done = limit;
}

/* test whether m satisfies sigma(m) > m + phi(m) --------------------- */
static bool good_odd(uint64_t m) {
    uint64_t orig = m;
    uint64_t tmp = m;
    __uint128_t sigma = 1;
    __uint128_t phi   = orig;

    idx_t sqrt_lim = int_sqrt_u64(orig);
    ensure_primes_upto(sqrt_lim);

    for (size_t i = 0; i < prime_len && (uint64_t)prime_arr[i] * prime_arr[i] <= tmp; ++i) {
        uint64_t p = prime_arr[i];
        if (tmp % p == 0) {
            uint64_t e = 0;
            uint64_t p_pow = 1;
            while (tmp % p == 0) {
                tmp   /= p;
                e++;
                p_pow *= p;
            }
            __uint128_t term = ((__uint128_t)p_pow * p - 1) / (p - 1);
            sigma *= term;
            phi = phi / p * (p - 1);
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        uint64_t q = tmp;
        sigma *= (q + 1);
        phi = phi / q * (q - 1);
    }
    return sigma > (__uint128_t)orig + phi;
}

/* cache for computed terms ------------------------------------------- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* extend cache to at least n terms ----------------------------------- */
static void extend_cache_to(idx_t n) {
    static idx_t last_checked = 1; /* start after 1, since 1 is not a term */
    while (cache_len < n) {
        last_checked += 2;                /* only odd numbers */
        if (good_odd((uint64_t)last_checked)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
}

/* public interface ---------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* out of range, return 0 */
    if (n > cache_len) extend_cache_to(n);
    return cache[n - 1];
}
