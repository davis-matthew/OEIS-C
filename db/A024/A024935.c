/*
 * A024935  Duplicate of A051034.
 * https://oeis.org/A024935
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1 if n is prime; 2 if n is even or n‑2 is prime; 3
 *            otherwise
 * Range:     i8 n <= 117, i16 n <= 117, i32 n <= 117, i64 n <= 117,
 *            i128 n <= 117, big any n
 * Helpers:   mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mulmod.h"

/* modular multiplication using 128 bit to avoid overflow */

/* modular exponentiation */
static inline uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    uint64_t acc = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = mulmod(result, acc, mod);
        acc = mulmod(acc, acc, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* deterministic Miller‑Rabin for 64‑bit integers */
static inline int is_prime_uint64(uint64_t n) {
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (int i = 0; i < 7; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int passed = 0;
        for (int r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
            if (x == n - 1) { passed = 1; break; }
        }
        if (!passed) return 0;
    }
    return 1;
}

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;          /* undefined below offset */
    if (is_prime_uint64(n))                return (seq_t)1;
    if ((n & 1ULL) == 0ULL)                return (seq_t)2;
    if (is_prime_uint64(n - 2ULL))         return (seq_t)2;
    return (seq_t)3;
}
