/*
 * A091666  Difference between prime(n)^2 and the next prime.
 * https://oeis.org/A091666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = next_prime(prime(n)^2) – prime(n)^2 * where prime(n) is
 *            the n‑th prime (offset 1). * * The routine generates primes by
 *            trial division with the * primes already found. It uses a
 *            deterministic Miller–Rabin * test for 64‑bit integers to search
 *            for the next prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mul_mod64.h"

/* Maximum number of primes we keep.  200 000 is far more than needed
 * for all n used in the tests (the 100 000‑th prime is 1 298 193). */
#define MAX_PRIME_COUNT 200000

/* Array of primes and current counter. */
static uint32_t prime_list[MAX_PRIME_COUNT];
static size_t   prime_count = 0;

/* Fast 64‑bit modular multiplication (uses 128‑bit intermediate). */

/* Fast 64‑bit modular exponentiation. */
static inline uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit integers. */
static int is_prime64(uint64_t n) {
    if (n < 2)                 return 0;
    if (n == 2 || n == 3)      return 1;
    if ((n & 1) == 0)          return 0;
    if ((n % 3) == 0)          return 0;

    /* trial division by small primes */
    static const uint32_t small_primes[] = {5,7,11,13,17,19,23,29,31,37,41,43,47};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint32_t p = small_primes[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return 0;
    }

    /* write n‑1 as d·2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* bases sufficient for deterministic test up to 2^64 */
    static const uint64_t bases[] = {
        2ULL,  325ULL,  9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL, 0
    };

    for (size_t i = 0; bases[i] != 0; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int r;
        for (r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) break;
        }
        if (r == s) return 0;
    }
    return 1;
}

/* Append enough primes to prime_list to reach at least target_count. */
static void ensure_primes(size_t target_count) {
    if (target_count <= prime_count) return;

    if (prime_count == 0) {
        prime_list[prime_count++] = 2;
    }
    if (prime_count == 1) {
        prime_list[prime_count++] = 3;
    }

    uint32_t candidate = prime_list[prime_count - 1] + 2; /* skip even */
    while (prime_count < target_count && prime_count < MAX_PRIME_COUNT) {
        int is_p = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > candidate) break;
            if (candidate % p == 0) { is_p = 0; break; }
        }
        if (is_p) prime_list[prime_count++] = candidate;
        candidate += 2;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* safety for out‑of‑range calls */

    /* Ensure that we have at least n primes. */
    ensure_primes((size_t)n);

    uint32_t p = prime_list[(size_t)n - 1];
    uint64_t p_sq = (uint64_t)p * (uint64_t)p;

    /* Search for the first prime > p^2.  We start at p^2+1
     * and step by 2 to skip even numbers (p^2 is odd for p>2). */
    uint64_t candidate = p_sq + 1;
    if (candidate % 2 == 0) candidate++;   /* ensure odd */

    while (!is_prime64(candidate)) {
        candidate += 2;
    }

    return (seq_t)(candidate - p_sq);
}
