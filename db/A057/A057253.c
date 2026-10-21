/*
 * A057253  Numbers n such that n | 9^n + 8^n + 7^n + 6^n + 5^n.
 * https://oeis.org/A057253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 32, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A057253: integers n such that n | 9ⁿ + 8ⁿ + 7ⁿ + 6ⁿ + 5ⁿ
 * The function scans successive integers, testing the divisibility
 * using modular exponentiation (fast repeated squaring).  It counts
 * the numbers that satisfy the condition until the desired index
 * n is reached, then returns that number.  The implementation
 * is O(n·log n) in the value of n and works for any seq_t and
 * idx_t types supplied by the build environment. */

#include <stdint.h>

/* modular multiplication using 128‑bit arithmetic when available */
static inline uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
#if defined(__SIZEOF_INT128__)
    return (unsigned __int128)a * b % mod;
#else
    /* fallback that runs in O(log 64) time – slower but portable */
    uint64_t result = 0;
    a %= mod;
    while (b) {
        if (b & 1ULL) result = (result + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return result;
#endif
}

/* modular exponentiation (base^exp mod mod) */
static inline uint64_t pow_mod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) res = mul_mod_u64(res, base, mod);
        base = mul_mod_u64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety; OEIS offset starts at 1 */

    idx_t hits = 0;
    for (idx_t m = 1; ; ++m) {
        uint64_t mod = m;
        uint64_t sum = 0;
        for (uint64_t b = 5; b <= 9; ++b)
            sum = (sum + pow_mod_u64(b, m, mod)) % mod;

        if (sum % mod == 0) {
            ++hits;
            if (hits == n) return (seq_t)m;
        }
    }
}
