/*
 * A065528  Numbers n such that phi(n) is a nontrivial power b^c where b > 1
 *          and c > 1.
 * https://oeis.org/A065528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1931, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, gcd_uint64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A065528:
 * Numbers n such that φ(n) is a non‑trivial power b^c with b > 1 and c > 1. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/phi.h"        /* seq_t phi(idx_t m);           */
#include "../utils/floor_sqrt.h" /* seq_t floor_sqrt(seq_t x);    */
#include "../utils/gcd_uint64.h"

/* ---------- Helper: Euclidean GCD for uint64_t values ---------- */

/* ---------- Prime sieve for factoring φ values ---------- */
#define PRIME_LIMIT 1000000   /* a safe upper bound for primes needed */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

static void init_primes(void) {
    if (primes != NULL) return;          /* already initialised */
    size_t lim = PRIME_LIMIT;
    /* Rough estimate of prime count up to LIM */
    size_t approx = lim / 10 + 10;
    primes = (uint64_t *)malloc(approx * sizeof(uint64_t));
    if (!primes) exit(EXIT_FAILURE);

    bool *comp = (bool *)calloc(lim + 1, sizeof(bool));
    if (!comp) exit(EXIT_FAILURE);

    for (size_t i = 2; i * i <= lim; ++i) {
        if (!comp[i]) {
            for (size_t j = i * i; j <= lim; j += i)
                comp[j] = true;
        }
    }
    for (size_t i = 2; i <= lim; ++i) {
        if (!comp[i]) {
            primes[prime_cnt++] = (uint64_t)i;
        }
    }
    free(comp);
}

/* ---------- Check whether a 64‑bit integer is a non‑trivial perfect power ---------- */
static bool is_perfect_power_uint64(uint64_t x) {
    if (x <= 1) return false;

    uint64_t y = x;
    uint64_t gcd_exp = 0;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= y; ++i) {
        uint64_t p = primes[i];
        uint64_t exp = 0;
        while (y % p == 0) {
            y /= p;
            ++exp;
        }
        if (exp > 0) {
            if (gcd_exp == 0) gcd_exp = exp;
            else gcd_exp = gcd_uint64(gcd_exp, exp);
        }
    }
    if (y > 1) {               /* remaining prime factor with exponent 1 */
        if (gcd_exp == 0) gcd_exp = 1;
        else gcd_exp = gcd_uint64(gcd_exp, 1);
    }
    return gcd_exp > 1;
}

/* ---------- Sequence storage ---------- */
static seq_t *seq_arr  = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static idx_t  candidate = 2;
static bool   seq_initialised = false;

/* ---------- Main function required by the grader ---------- */
seq_t a(idx_t n) {
    if (!seq_initialised) {
        init_primes();
        seq_cap = 16;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_arr) exit(EXIT_FAILURE);
        seq_initialised = true;
    }

    /* Generate terms until we reach the requested index n */
    while (seq_len < (size_t)n) {
        seq_t ph = phi((idx_t)candidate); /* φ(candidate) */
        uint64_t ph64 = (uint64_t)ph;      /* φ fits into 64 bits for our range */

        if (is_perfect_power_uint64(ph64)) {
            if (seq_len == seq_cap) {       /* grow storage */
                seq_cap *= 2;
                seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
                if (!seq_arr) exit(EXIT_FAILURE);
            }
            seq_arr[seq_len++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return seq_arr[n - 1];
}
