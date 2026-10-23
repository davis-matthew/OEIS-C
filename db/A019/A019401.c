/*
 * A019401  Primes with primitive root 78.
 * https://oeis.org/A019401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A019401: Primes p for which 78 is a primitive root modulo p    */
/* The function a(idx_t n) returns the nth such prime (a(1)=5).             */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/next_prime.h"
#include "../utils/build_primes_upto.h"

static const idx_t SMALL_PRIME_LIMIT = 100000;
static uint64_t *small_primes = NULL;
static size_t small_cnt = 0;
static bool primes_initialized = false;

static void init_small_primes(void)
{
    if (!primes_initialized) {
        build_primes_upto(SMALL_PRIME_LIMIT, &small_primes, &small_cnt);
        primes_initialized = true;
    }
}

static bool is_primitive_root_78(uint64_t p)
{
    /* 78 is not coprime to primes 2,3,13. */
    if (p == 2 || p == 3 || p == 13) return false;

    uint64_t phi = p - 1;
    uint64_t m = phi;

    for (size_t i = 0; i < small_cnt && small_primes[i] * small_primes[i] <= m; ++i) {
        uint64_t q = small_primes[i];
        if (m % q == 0) {
            if (mod_pow((seq_t)78u, phi / q, (seq_t)p) == (seq_t)1) return false;
            while (m % q == 0) m /= q;
        }
    }
    if (m > 1) {
        if (mod_pow((seq_t)78u, phi / m, (seq_t)p) == (seq_t)1) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset; harmless */
    init_small_primes();

    uint64_t count = 0;
    uint64_t candidate = 1;
    while (count < n) {
        candidate = next_prime(candidate);
        if (is_primitive_root_78(candidate)) {
            ++count;
        }
    }
    return (seq_t)candidate;
}
