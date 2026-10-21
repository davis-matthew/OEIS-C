/*
 * A051091  Primes p such that x^44 = -2 has a solution mod p.
 * https://oeis.org/A051091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: (x^44) ≡ -2 (mod p) over primes p.  
   For a prime p, let g = gcd(44, p-1).  
   Then a solution exists iff (p-2)^((p-1)/g) ≡ 1 (mod p). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd_uint64.h"

/* ---------- Local helpers --------------------------------------------- */

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t res = 1;
    a %= mod;
    while (e) {
        if (e & 1)
            res = (unsigned __int128)res * a % mod;
        a = (unsigned __int128)a * a % mod;
        e >>= 1;
    }
    return res;
}

/* ---- Prime list (dynamic, 32‑bit entries). --------------------------- */
static uint32_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;

static void ensure_prime_capacity(void) {
    if (prime_cnt >= prime_cap) {
        size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

static bool is_prime_uint32(uint32_t x) {
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

static void add_prime(uint32_t p) {
    ensure_prime_capacity();
    prime_list[prime_cnt++] = p;
}

/* ---- Condition tester ------------------------------------------------- */
static bool satisfies_prime(uint32_t p) {
    if (p == 2) return true; /* 0^44 ≡ 0 ≡ -2 (mod 2) */
    uint64_t g  = gcd_uint64(44ULL, (uint64_t)p - 1ULL);
    uint64_t exp = ((uint64_t)p - 1ULL) / g;
    uint64_t base = (uint64_t)p - 2ULL;
    return mod_pow(base, exp, p) == 1ULL;
}

/* ---------- Exported function ------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* undefined index */

    /* Initialise prime list with 2 once. */
    if (prime_cnt == 0) add_prime(2);

    size_t found = 0;
    uint32_t candidate = 2;

    for (;;) {
        if (is_prime_uint32(candidate)) {
            if (satisfies_prime(candidate)) {
                ++found;
                if (found == (size_t)n) {
                    return (seq_t)candidate;
                }
            }
            add_prime(candidate);
        }
        /* advance to next odd number after 2 */
        candidate += (candidate == 2) ? 1 : 2;
    }
}
