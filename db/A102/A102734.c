/*
 * A102734  Primes of the form 23n+5.
 * https://oeis.org/A102734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 23k+5 (k ≥ 0)
 * Range:     u8 n <= 2, u16 n <= 302, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul_u64, mod_pow_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_mul_u64.h"
#include "../utils/mod_pow_u64.h"

static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2u) return false;
    static const uint64_t small_primes[] = {
        2u,3u,5u,7u,11u,13u,17u,19u,23u,29u,31u,37u,41u,43u,47u,53u,59u,61u,67u,71u
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0u) return false;
    }

    /* Miller–Rabin witnesses for <2^64 */
    static const uint32_t witnesses[] = {
        2u,325u,9375u,28178u,450775u,9780504u,1795265022u
    };
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1u) == 0u) {
        d >>= 1;
        s++;
    }
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0u) continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1u || x == n - 1u) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1u) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; no defined behaviour for n==0 */
    idx_t count = 0;
    idx_t k = 0;
    seq_t candidate;
    while (count < n) {
        candidate = (seq_t)23 * (seq_t)k + (seq_t)5;
        /* test primality using 64‑bit routine */
        if (is_prime_uint64((uint64_t)candidate)) {
            count++;
            if (count == n)
                return candidate;
        }
        k++;
    }
    /* unreachable */
    return (seq_t)0;
}
