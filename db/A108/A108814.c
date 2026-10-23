/*
 * A108814  Numbers k such that k^4 + 4 is semiprime.
 * https://oeis.org/A108814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where k is the n-th positive integer such that both
 *            k^2-2k+2 and k^2+2k+2 are prime (i.e. k^4+4 is a semiprime)
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* 64‑bit deterministic Miller–Rabin primality test.
   Works for all numbers < 3.4×10^14 (our use‑case is < 10^12). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* Small primes */
    static const uint64_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Write n-1 as d*2^s */
    uint64_t d = n-1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Bases sufficient for n < 3.4e14 */
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;

        /* modular exponentiation: a^d mod n */
        __uint128_t x = 1, base = a % n;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }
        uint64_t x64 = (uint64_t)x;
        if (x64 == 1 || x64 == n-1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            x64 = (uint64_t)x;
            if (x64 == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }

    return true;
}

/* Cache the generated terms */
static seq_t *terms = NULL;
static size_t terms_count = 0;
static size_t terms_cap   = 0;
static uint64_t next_k = 3;  /* start with first odd candidate */

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but guard */

    /* Expand storage if necessary */
    if (terms_count < n) {
        if (terms_cap == 0) terms_cap = 16;
        while (terms_cap < n) terms_cap <<= 1;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        if (!terms) exit(EXIT_FAILURE);

        /* Generate until we have n terms */
        while (terms_count < n) {
            /* Compute p = k^2 - 2k + 2, q = k^2 + 2k + 2 */
            uint64_t k = next_k;
            next_k += 2;   /* skip even k > 2 */

            __uint128_t kk = (__uint128_t)k * k;
            uint64_t p = (uint64_t)(kk - 2*k + 2);
            uint64_t q = (uint64_t)(kk + 2*k + 2);

            if (is_prime_uint64(p) && is_prime_uint64(q)) {
                terms[terms_count++] = (seq_t)k;
            }
        }
    }

    return terms[n - 1];
}
