/*
 * A090920  Primes of the form n followed by the least k == 1 (mod n).
 * https://oeis.org/A090920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 4316, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A090920: for each n, the concatenation of n with the smallest
   k≡1 (mod n) that yields a prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* fast modular multiplication (uses __int128 for safety) */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

/* fast modular exponentiation */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic Miller–Rabin primality test for 64‑bit integers
   (bases 2,3,5,7,11,13,17 are sufficient for n < 2^64). */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof small[0]; ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* compute 10^e as seq_t */
static inline seq_t pow10_seq(idx_t e) {
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i) r *= 10;
    return r;
}

/* digit count provided by the utilities */
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1 */
    idx_t m = 0;
    while (true) {
        idx_t k = 1 + m * n;           /* k ≡ 1 (mod n) */
        uint64_t exp = digit_count((uint64_t)k);   /* number of decimal digits of k */
        seq_t cand = (seq_t)n * pow10_seq(exp) + (seq_t)k;
        uint64_t cand64 = (uint64_t)cand;  /* fits in 64 bit for n ≤ 10 000 */
        if (is_prime_uint64(cand64)) return cand;
        ++m;
        /* safety guard: should not be needed for valid n */
        if (m >= n) break;
    }
    return 0;                           /* unreachable for valid inputs */
}
