/*
 * A173757  Numbers k such that exactly one of k^2 + k + 1 and k^2 + k + 11 is
 *          prime.
 * https://oeis.org/A173757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A173757: numbers k such that exactly one of k² + k + 1 and k² + k + 11
   is prime.  Computed by a simple search. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Modular multiplication (a*b) % mod using 128‑bit intermediate to avoid overflow. */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

/* Binary exponentiation: a^d mod mod. */
static inline uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t result = 1;
    a %= mod;
    while (d) {
        if (d & 1) result = mul_mod(result, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin primality test for 64‑bit unsigned integers.
   Valid for all n < 3,474,749,660,383 which comfortably includes all values
   needed for n up to about 10,000 in this sequence. */
static inline bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    size_t spn = sizeof(small_primes) / sizeof(small_primes[0]);
    for (size_t i = 0; i < spn; ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13};
    size_t bn = sizeof(bases) / sizeof(bases[0]);
    for (size_t i = 0; i < bn; ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t found = 0;
    idx_t k = 0;
    while (1) {
        /* compute k² + k + 1 and +11 using 128‑bit intermediate to avoid overflow */
        unsigned __int128 prod = (unsigned __int128)k * (k + 1);
        uint64_t v1 = (uint64_t)(prod + 1);
        uint64_t v2 = (uint64_t)(prod + 11);

        bool p1 = is_prime_uint64(v1);
        bool p2 = is_prime_uint64(v2);

        if (p1 ^ p2) {            /* exactly one is prime */
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
