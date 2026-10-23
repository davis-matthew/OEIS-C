/*
 * A108900  Numbers k such that 2357*k + 11 is prime.
 * https://oeis.org/A108900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 2357*k + 11 is prime. */

/* Helper: deterministic Miller‑Rabin primality test for 64‑bit numbers. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Multiply a*b modulo mod using 128‑bit arithmetic. */
static inline uint64_t mulmod_uint64(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* Power a^e mod mod. */
static uint64_t powmod_uint64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t result = 1;
    while (e) {
        if (e & 1)
            result = mulmod_uint64(result, a, mod);
        a = mulmod_uint64(a, a, mod);
        e >>= 1;
    }
    return result;
}

/* Deterministic Miller‑Rabin for 64‑bit integers. */
static bool is_prime_candidate(uint64_t n) {
    if (n < 2)
        return false;
    /* Small prime trial division */
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37
    };
    for (size_t i = 0; i < 12; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }
    /* Miller‑Rabin bases sufficient for <2^64 */
    static const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    for (size_t i = 0; i < 7; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = powmod_uint64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mulmod_uint64(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (cont)
            continue;
        return false;
    }
    return true;
}

/* Static storage for found k values. */
static uint64_t *k_vals   = NULL; /* k values (even only) */
static idx_t    k_size    = 0;    /* number of stored k's */
static idx_t    k_cap     = 0;    /* current capacity */
static idx_t    next_k    = 0;    /* next even k to test */

/* Ensure the array has room for at least 'need' elements. */
static void ensure_capacity(idx_t need) {
    if (k_cap >= need)
        return;
    if (!k_cap)
        k_cap = 4;
    while (k_cap < need)
        k_cap <<= 1;
    k_vals = (uint64_t *)realloc(k_vals, k_cap * sizeof(uint64_t));
}

/* Returns the n-th term of the sequence (1‑based). */
seq_t a(idx_t n) {
    while (k_size < n) {
        uint64_t candidate = 2357ULL * (uint64_t)next_k + 11ULL;
        if (is_prime_candidate(candidate)) {
            ensure_capacity(k_size + 1);
            k_vals[k_size++] = (uint64_t)next_k;
        }
        next_k += 2;          /* only even k can work */
    }
    return (seq_t)k_vals[n - 1];
}
