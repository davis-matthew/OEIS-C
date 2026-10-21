/*
 * A068560  Numbers k such that phi(k) = tau(k)^2.
 * https://oeis.org/A068560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A068560:  numbers k such that φ(k) = τ(k)^2. 
   The sequence is finite; we generate all terms up to a fixed upper
   bound and store them in an array.  The function a(n) returns the
   n-th term (1-indexed).  The first call performs the generation,
   subsequent calls use the cached array. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_N 2000000 /* safe upper bound for the search range */

static uint32_t *spf = NULL;          /* smallest prime factor array */
static seq_t   *seq_arr = NULL;       /* array of found sequence terms */
static size_t   seq_len = 0;
static size_t   seq_cap = 0;
static bool     initialized = false;

/* Linear sieve to fill spf up to MAX_N */
static void build_spf(void) {
    spf = (uint32_t *)malloc((MAX_N + 1) * sizeof(uint32_t));
    if (!spf) return;
    for (uint32_t i = 0; i <= MAX_N; ++i) spf[i] = 0;
    uint32_t *primes = (uint32_t *)malloc((MAX_N/10) * sizeof(uint32_t));
    size_t pc = 0;
    for (uint32_t i = 2; i <= MAX_N; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[pc++] = i;
        }
        for (size_t j = 0; j < pc && primes[j] <= spf[i] && i * primes[j] <= MAX_N; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
    free(primes);
}

/* Compute φ and τ for n using the spf array.  Return true if φ(n) = τ(n)^2. */
static bool satisfies(uint32_t n) {
    if (n == 1) return true; /* φ(1)=1, τ(1)=1 */
    uint32_t tmp = n;
    uint64_t tau = 1;
    uint64_t phi = n;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        uint32_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            exp++;
        }
        tau *= (uint64_t)(exp + 1);
        if (phi % p == 0) phi -= phi / p; /* φ = φ * (1-1/p) */
    }
    uint64_t tau_sq = tau * tau;
    return phi == tau_sq;
}

/* Generate all terms up to MAX_N and store in seq_arr. */
static void generate_sequence(void) {
    seq_cap = 64;
    seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq_arr) return;
    seq_len = 0;
    /* ensure spf is built */
    if (!spf) build_spf();
    for (uint32_t i = 1; i <= MAX_N; ++i) {
        if (satisfies(i)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
            }
            seq_arr[seq_len++] = (seq_t)i;
        }
    }
}

seq_t a(idx_t n) {
    if (!initialized) {
        generate_sequence();
        initialized = true;
    }
    if (n == 0 || n > seq_len) return 0; /* out of bounds */
    return seq_arr[n - 1];
}
