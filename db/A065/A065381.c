/*
 * A065381  Primes not of the form p + 2^k, p prime and k >= 0.
 * https://oeis.org/A065381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 841, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes not of the form p + 2^k (p prime, k ≥ 0) */
/* a(n) is the nth such prime. The implementation uses a deterministic
   Miller–Rabin test for 64‑bit integers and increments through the
   integers checking primality and representability. */

#include <stdint.h>
#include <stddef.h>

static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}
static inline uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    uint64_t cur = base % mod;
    while (exp) {
        if (exp & 1) res = mul_mod(res, cur, mod);
        cur = mul_mod(cur, cur, mod);
        exp >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin for 64‑bit integers.
   Works for all n < 2^64. */
static int is_prime_uint64(uint64_t n) {
    if (n < 2) return 0;
    /* small primes check */
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return 1;
        if (n % p == 0) return 0;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* witnesses for 64‑bit deterministic test */
    static const uint64_t witnesses[] = {2ULL, 325ULL, 9375ULL,
                                         28178ULL, 450775ULL,
                                         9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;          /* witness equal to n */
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int r;
        for (r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) break;
        }
        if (r == s) return 0;          /* composite */
    }
    return 1;                          /* probably prime */
}

/* a(n) – nth prime that cannot be written as q + 2^k with q prime, k≥0 */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;       /* first term is 2 */

    idx_t count = 1;                  /* we already have 2 */
    uint64_t q = 3;                    /* next odd candidate */

    while (1) {
        if (is_prime_uint64(q)) {
            int representable = 0;
            for (uint64_t pow = 1; pow < q && pow; pow <<= 1) {
                uint64_t diff = q - pow;
                if (diff >= 2 && is_prime_uint64(diff)) {
                    representable = 1;
                    break;
                }
                if (pow > UINT64_MAX / 2) break; /* avoid overflow */
            }
            if (!representable) {
                ++count;
                if (count == n) return (seq_t)q;
            }
        }
        q += 2;  /* skip even numbers */
    }
    return (seq_t)0;   /* unreachable */
}
