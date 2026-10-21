/*
 * A069204  Emirps congruent to their reversal mod 4.
 * https://oeis.org/A069204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 475, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Emirps congruent to their reversal mod 4 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* modular multiplication using 128‑bit intermediate */
static inline unsigned long long mod_mul(unsigned long long a,
                                         unsigned long long b,
                                         unsigned long long mod) {
    return (unsigned long long)((__uint128_t)a * b % mod);
}

/* modular exponentiation */
static inline unsigned long long mod_pow(unsigned long long base,
                                         unsigned long long exp,
                                         unsigned long long mod) {
    unsigned long long result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime_uint64(unsigned long long n) {
    if (n < 2) return false;
    static const unsigned long long small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0ULL) return false;
    }

    unsigned long long d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    static const unsigned long long bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        unsigned long long a = bases[i] % n;
        if (a == 0ULL) continue;
        unsigned long long x = mod_pow(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool composite = true;
        for (unsigned long long r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* decimal reversal, safe for 64‑bit values */
static inline unsigned long long rev_digits_uint64(unsigned long long x) {
    unsigned long long rev = 0ULL;
    while (x) {
        rev = rev * 10ULL + (x % 10ULL);
        x /= 10ULL;
    }
    return rev;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* offset starts at 1 */

    idx_t found = 0ULL;
    unsigned long long candidate = 2ULL;

    while (true) {
        if (!is_prime_uint64(candidate)) { ++candidate; continue; }

        unsigned long long rev = rev_digits_uint64(candidate);
        if (rev == candidate) { ++candidate; continue; }   /* palindrome */

        if (!is_prime_uint64(rev)) { ++candidate; continue; }

        if ( (candidate & 3ULL) != (rev & 3ULL) ) { ++candidate; continue; }

        ++found;
        if (found == n) return (seq_t)candidate;
        ++candidate;
    }
}
