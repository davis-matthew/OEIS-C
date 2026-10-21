/*
 * A074110  Largest n-digit squarefree number.
 * https://oeis.org/A074110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = largest n‑digit squarefree number. The value is 10^n – k
 *            where k is the smallest integer ≥2 such that 10^n – k is
 *            squarefree
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"

/* Prime bound for testing square‑factors. */
#define PRIME_LIMIT 200000

static unsigned int primes[20000];
static size_t primeCnt = 0;

/* Initialise list of primes ≤ PRIME_LIMIT using a simple sieve. */
static void init_primes(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    bool *is_comp = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(*is_comp));
    if (!is_comp) return;              /* allocation failed – unlikely */
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_comp[i] = false;

    for (unsigned int p = 2; (uint64_t)p * p <= PRIME_LIMIT; ++p) {
        if (!is_comp[p]) {
            for (unsigned int j = p * p; j <= PRIME_LIMIT; j += p) {
                is_comp[j] = true;
            }
        }
    }

    for (unsigned int p = 2; p <= PRIME_LIMIT; ++p) {
        if (!is_comp[p]) {
            primes[primeCnt++] = p;
        }
    }
    free(is_comp);
}

/* 128‑bit multiplication modulo mod. */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* Compute 10^exp (mod mod).  exp is at most idx_t (≤ 10000).  */
static uint64_t pow10_mod(idx_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t base   = 10 % mod;
    idx_t e = exp;
    while (e) {
        if (e & 1) result = mul_mod_uint64(result, base, mod);
        base = mul_mod_uint64(base, base, mod);
        e >>= 1;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_primes();

    /* Base value 10^n as seq_t. */
    seq_t base = pow10(n);

    /* k = 2 initially; cand = base - k. */
    idx_t k = 2;

    for (;;) {
        seq_t cand = base - (seq_t)k;

        /* Test whether cand has a square divisor. */
        bool has_sq = false;
        for (size_t i = 0; i < primeCnt; ++i) {
            uint64_t p   = primes[i];
            uint64_t mod = p * p;          /* 0 < mod ≤ (200000)^2 < 2^63 */
            uint64_t val = pow10_mod(n, mod);  /* 10^n mod mod */

            /* Check if 10^n ≡ k (mod p^2). */
            if (val == (uint64_t)k % mod) {
                has_sq = true;
                break;
            }
        }

        /* If cand is squarefree, return it.  Otherwise try next k. */
        if (!has_sq) return cand;
        ++k;      /* cand becomes base - (k+1) on next iteration */
    }
}
