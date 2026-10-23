/*
 * A190052  Least number having exactly two odd prime factors that differ by
 *          2*n^2.
 * https://oeis.org/A190052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 33, u32 n <= 3126, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): least integer that is a product of two odd primes
   whose difference is 2 * n²  (OEIS A190052) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* 64‑bit deterministic Miller–Rabin primality test (bases valid for all
   64‑bit unsigned integers).  */
static uint64_t mod_mul_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = mod_mul_u64(result, base, mod);
        base = mod_mul_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint32_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* write n-1 = d * 2^s  with d odd */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint32_t bases[] = {
        2,325,9375,28178,450775,9780504,1795265022
    }; /* deterministic for 64-bit */
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a < 2) continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n = 0, but safe return */
    /* compute difference d = 2 * n^2   using 128‑bit to avoid overflow */
    unsigned __int128 tn = (unsigned __int128)n;
    unsigned __int128 d128 = 2ULL * tn * tn;
    uint64_t d = (uint64_t)d128;   /* fits for all n we are expected to handle */

    uint64_t p = 3;  /* smallest odd prime */
    while (1) {
        if (is_prime_u64(p) && is_prime_u64(p + d)) {
            return (seq_t)p * (seq_t)(p + d);
        }
        p += 2;      /* skip even numbers */
    }
}
