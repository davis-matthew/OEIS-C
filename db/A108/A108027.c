/*
 * A108027  Numbers k such that 137*k + 139 is prime.
 * https://oeis.org/A108027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that 137*k + 139 is prime
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* 64‑bit modular multiplication avoiding overflow */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__int128)a * b % mod);
}

/* 64‑bit modular exponentiation */
static uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit integers */
static bool isPrime(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                            19ULL,23ULL,29ULL,31ULL,37ULL,0ULL};
    for (int i = 0; small_primes[i]; ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,
                                     450775ULL,9780504ULL,1795265022ULL,0ULL};
    for (int i = 0; bases[i]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Array of computed sequence terms */
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static idx_t next_k = 0;

/* Compute the nth term (1‑based) */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset 1: n>=1 intended */
    while (seq_len < n) {
        uint64_t k = next_k;
        uint64_t val = 137ULL * k + 139ULL;
        if (isPrime(val)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)k;
        }
        ++next_k;
    }
    return seq[n - 1];
}
