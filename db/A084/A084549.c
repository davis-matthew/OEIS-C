/*
 * A084549  Numbers k that have primitive roots less than k that are
 *          nonnegative perfect seventh powers.
 * https://oeis.org/A084549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 4417, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64, seventh_root_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A084549 – Numbers k that have primitive roots less than k that are nonnegative perfect seventh powers. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd64.h"
#include "../utils/seventh_root_floor.h"

#define SIEVE_MAX 5000000   /* enough for n up to ≈10000 */

static int *spf = NULL;     /* smallest prime factor array */

/* Initialize the sieve of smallest prime factors */
static void init_sieve(void) {
    if (spf != NULL) return;
    spf = (int*)malloc((SIEVE_MAX + 1) * sizeof(int));
    for (int i = 0; i <= SIEVE_MAX; ++i) spf[i] = i;
    for (int i = 2; (int64_t)i * i <= SIEVE_MAX; ++i) {
        if (spf[i] == i) {
            for (int j = i * i; j <= SIEVE_MAX; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
}

/* Compute gcd */

/* Fast modular exponent (uint64_t) */
static inline uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

/* Compute φ(n) using the spf table */
static uint64_t phi(uint64_t n) {
    uint64_t result = n;
    while (n > 1) {
        int p = spf[n];
        while (n % p == 0) n /= p;
        result -= result / p;
    }
    return result;
}

/* Check if a number is a prime power: only one distinct prime factor */
static bool is_prime_power(uint64_t n) {
    if (n < 2) return false;
    int cnt = 0;
    while (n > 1) {
        int p = spf[n];
        ++cnt;
        while (n % p == 0) n /= p;
        if (cnt > 1) return false;
    }
    return true;
}

/* Does k have primitive roots? */
static bool has_primitive_root(uint64_t k) {
    if (k == 2 || k == 4) return true;
    if ((k & 1) == 0) {          /* even k */
        if (k % 4 == 0) return false;   /* 4 divides k -> no primitive root */
        uint64_t m = k / 2;
        return is_prime_power(m);
    }
    return is_prime_power(k);
}

/* Check if g is a primitive root modulo k (k>1) */
static bool is_primitive_root(uint64_t g, uint64_t k) {
    if (g % k == 0) return false;
    uint64_t ph = phi(k);
    uint64_t temp = ph;
    while (temp > 1) {
        int p = spf[temp];
        while (temp % p == 0) temp /= p;
        if (modpow64(g, ph / p, k) == 1) return false;
    }
    return true;
}

/* Search for the nth term */
seq_t a(idx_t n) {
    static idx_t next_k = 2;        /* next k to test */
    static uint64_t *terms = NULL;  /* stored sequence values */
    static size_t len = 0;
    static size_t cap = 0;

    init_sieve();  /* ensure sieve is ready */

    /* Grow terms buffer if needed */
    if (cap == 0) {
        cap = 1024;
        terms = (uint64_t*)malloc(cap * sizeof(uint64_t));
    }
    while (len < (size_t)n) {
        if (has_primitive_root(next_k)) {
            uint64_t limit = seventh_root_floor(next_k);
            for (uint64_t s = 1; s <= limit; ++s) {   /* 0 gives 0 which can't be coprime */
                /* compute s^7 */
                uint64_t g = 1;
                for (int i = 0; i < 7; ++i) g *= s;
                if (g >= next_k) break;
                if (gcd64(g, next_k) != 1) continue;
                if (is_primitive_root(g, next_k)) {
                    /* record k */
                    if (len == cap) { cap *= 2; terms = (uint64_t*)realloc(terms, cap * sizeof(uint64_t)); }
                    terms[len++] = next_k;
                    break;
                }
            }
        }
        ++next_k;
    }
    return (seq_t)terms[n - 1];
}
