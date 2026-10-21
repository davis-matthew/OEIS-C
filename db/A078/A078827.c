/*
 * A078827  Number of primes contained as binary substrings in binary
 *          representation of n, counted with repetitions.
 * https://oeis.org/A078827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078827:  Number of primes contained as binary substrings
   in the binary representation of n, counted with repetitions.
   Only substrings that start with a '1' are considered (to avoid leading
   zeros).  The function returns 0 for n = 0. */
#include <stdint.h>

/* Deterministic Miller–Rabin primality test for 64‑bit integers. */
static int is_prime_uint64(uint64_t x) {
    if (x < 2) return 0;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (int i = 0; i < 12; i++) {
        if (x % small_primes[i] == 0) return x == small_primes[i];
    }
    /* Write x−1 as d·2^s with d odd. */
    uint64_t d = x - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    /* Witnesses sufficient for all 64‑bit numbers. */
    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (int i = 0; i < 7; i++) {
        uint64_t a = witnesses[i] % x;
        if (a == 0) continue;
        __uint128_t t = 1, base = a, exp = d, mod = x;
        while (exp) {
            if (exp & 1) t = (t * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        uint64_t modpow = (uint64_t)t;
        if (modpow == 1 || modpow == x - 1) continue;
        int cont = 0;
        for (uint64_t r = 1; r < s; r++) {
            t = (__uint128_t)modpow * modpow % mod;
            modpow = (uint64_t)t;
            if (modpow == x - 1) { cont = 1; break; }
        }
        if (!cont) return 0;
    }
    return 1;
}

/* Length in bits of a 64‑bit unsigned integer.
   For n==0 it returns 1 (the single bit '0'). */
static idx_t bitlen_u64(idx_t n) {
    idx_t len = 0;
    if (n == 0) return 1;
    while (n) { len++; n >>= 1; }
    return len;
}

seq_t a(idx_t n) {
    /* Special case: n == 0 has no '1' bits. */
    if (n == 0) return 0;

    idx_t len = bitlen_u64(n);
    unsigned char bits[64];
    idx_t tmp = n;
    for (idx_t i = 0; i < len; i++)
        bits[i] = (tmp >> (len - 1 - i)) & 1;

    seq_t count = 0;
    for (idx_t start = 0; start < len; start++) {
        if (bits[start] == 0) continue;          /* refuse leading zeros */
        uint64_t val = 0;
        for (idx_t end = start; end < len; end++) {
            val = (val << 1) | bits[end];
            if (is_prime_uint64(val))
                count += 1;
        }
    }
    return count;
}
