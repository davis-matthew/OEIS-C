/*
 * A090195  Primes which when multiplied by their largest digit and 1 is
 *          subtracted form another prime.
 * https://oeis.org/A090195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n‑th prime p such that p * D(p) – 1 is also prime,
 *            where D(p) is the largest decimal digit of p
 * Range:     u8 n <= 5, u16 n <= 206, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_digit_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/largest_digit_uint64.h"

/* Multiply two 64‑bit unsigned numbers modulo mod (using 128‑bit). */
static inline uint64_t mulmod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* 64‑bit modular exponentiation. */
static uint64_t modpow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mulmod_uint64(r, a, mod);
        a = mulmod_uint64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin test for all 64‑bit unsigned integers. */
static int isPrimeUint64(uint64_t n)
{
    if (n < 2) return 0;
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow_uint64(a, d, n);
        if (x == 1 || x == n-1) continue;
        int comp = 1;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod_uint64(x, x, n);
            if (x == n-1) { comp = 0; break; }
        }
        if (comp) return 0;
    }
    return 1;
}

/* ---------- sequence storage ---------- */
static seq_t *terms   = NULL;   /* array of computed terms */
static size_t terms_cnt = 0;    /* number of terms stored */
static size_t terms_cap = 0;    /* current capacity of array */
static uint64_t last_prime = 1; /* prime that was last examined */

/* Initialise the dynamic array once. */
static void init_terms(void)
{
    if (terms != NULL) return;
    terms_cap = 1024;
    terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    init_terms();

    while (terms_cnt < (size_t)n) {
        /* Find next prime p greater than last_prime. */
        uint64_t p = last_prime + 1;
        while (!isPrimeUint64(p)) ++p;
        last_prime = p;

        /* Compute largest decimal digit of p and test p*d-1. */
        int d = largest_digit_uint64(p);
        uint64_t q = p * (uint64_t)d - 1;
        if (isPrimeUint64(q)) {
            /* Store p (not q) as a sequence term. */
            if (terms_cnt >= terms_cap) {
                terms_cap *= 2;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_cnt++] = (seq_t)p;
        }
    }
    return terms[n - 1];
}
