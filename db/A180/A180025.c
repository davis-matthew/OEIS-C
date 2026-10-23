/*
 * A180025  Prime numbers 3*n-2 such that n, 2*n-1 and 3*n-2 are prime.
 * https://oeis.org/A180025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A180025: 3*n-2 such that n, 2*n-1 and 3*n-2 are prime.
   The function computes the n‑th term by sieving up to a bound
   that grows roughly linearly with n (≈6 000 × n).  */
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif

/* Odd–number bitset helper -------------------------------------------------- */
static inline bool is_composite_odd(uint64_t x, uint8_t *bits) {
    uint64_t idx = (x - 3) >> 1;          /* index of odd number x */
    return (bits[idx >> 3] >> (idx & 7)) & 1;
}

static inline void set_composite_odd(uint64_t x, uint8_t *bits) {
    uint64_t idx = (x - 3) >> 1;
    bits[idx >> 3] |= 1 << (idx & 7);
}

/* Deterministic Miller–Rabin for 64‑bit integers ------------------------------ */
static uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t mod_pow_uint64(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mod_mul_uint64(r, a, mod);
        a = mod_mul_uint64(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool is_prime_ull(uint64_t n) {
    if (n < 2) return false;
    /* trial division by small primes */
    const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n-1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Miller–Rabin bases for deterministic test up to 2^64 */
    const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* --------------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset 1 – unused but harmless */

    /* Rough bound on the needed prime (over‑estimates but safe) */
    uint64_t p_limit = (uint64_t)((double)n * 6000.0);
    if (p_limit < 1000) p_limit = 1000;

    /* We need primes up to 3*p_limit + 2 for checking 3p-2 */
    uint64_t sieve_limit = p_limit * 3 + 2;

    /* Allocate bit‑set for odd numbers up to sieve_limit */
    size_t bits_cnt = (sieve_limit >> 1) + 1;       /* number of odd slots */
    size_t bytes_cnt = (bits_cnt + 7) >> 3;          /* bytes needed */
    uint8_t *bits = (uint8_t*)malloc(bytes_cnt);
    if (!bits) exit(1);                              /* allocation failure */

    /* Classic sieve of Eratosthenes on odd numbers */
    uint64_t sqrt_limit = (uint64_t)sqrt((long double)sieve_limit);
    for (uint64_t i = 3; i <= sqrt_limit; i += 2) {
        if (!is_composite_odd(i, bits)) {
            for (uint64_t j = i * i; j <= sieve_limit; j += i << 1)
                set_composite_odd(j, bits);
        }
    }

    size_t found = 0;   /* how many qualifying primes have been seen */

    /* Search primes up to p_limit using the sieve */
    for (uint64_t p = 3; p <= p_limit; p += 2) {
        if (is_composite_odd(p, bits)) continue;

        uint64_t v2 = 2 * p - 1;
        uint64_t v3 = 3 * p - 2;
        if (is_composite_odd(v2, bits) || is_composite_odd(v3, bits))
            continue;

        if (++found == n) {
            seq_t res = (seq_t)(3 * p - 2);
            free(bits);
            return res;
        }
    }

    /* If the bound was too small, continue with Miller–Rabin. */
    uint64_t p = p_limit + 1;
    if ((p & 1) == 0) ++p;   /* assure odd */

    for (;;) {
        if (is_prime_ull(p)) {
            uint64_t v2 = 2 * p - 1;
            uint64_t v3 = 3 * p - 2;
            if (is_prime_ull(v2) && is_prime_ull(v3)) {
                if (++found == n) {
                    seq_t res = (seq_t)(3 * p - 2);
                    free(bits);
                    return res;
                }
            }
        }
        p += 2;
    }
}
