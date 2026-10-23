/*
 * A134587  A Heron sequence with small terms starting from 60, 275, 325.
 * https://oeis.org/A134587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   A Heron sequence – a(n) is the smallest integer c > a(n‑1) *
 *            such that a(n‑2), a(n‑1), c are the side lengths of a Heron
 *            triangle. * The first three terms are 60, 275, 325. Each term is
 *            chosen * using the smallest possible larger value that makes the
 *            triple a * Heron triangle
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   toggle_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/toggle_prime.h"

#ifndef MAX_PRIME_FACTORS
#define MAX_PRIME_FACTORS 64
#endif

/* ---------- small prime sieve and prime list ----------
   The sieve is built on demand up to a fixed limit. */
static uint32_t *spf = NULL;          /* smallest prime factor table   */
static uint32_t *prime_list = NULL;   /* list of primes used in sieve */
static size_t   spf_size = 0;         /* number of entries in spf      */
static size_t   prime_count = 0;      /* number of primes found        */
static bool     spf_initialized = false;

static void init_spf(void)
{
    if (spf_initialized) return;

    /* A reasonable bound – 10 million – covers the vast majority of cases.
       Memory consumption: ~40 MB for 32‑bit idx_t, acceptable for the
       typical constraints of the problem. */
    const size_t limit = 10000000;
    spf_size = limit + 1;
    spf = (uint32_t *)calloc(spf_size, sizeof(uint32_t));
    if (!spf) abort();  /* insufficient memory */

    /* allocate an over‑estimate for the prime list to avoid reallocs */
    prime_list = (uint32_t *)malloc(limit / 12 * sizeof(uint32_t));
    if (!prime_list) abort();

    for (uint32_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            prime_list[prime_count++] = i;
        }
        for (size_t j = 0; j < prime_count; ++j) {
            uint32_t p = prime_list[j];
            uint64_t prod = (uint64_t)i * p;
            if (prod > limit) break;
            if (spf[prod] == 0) spf[prod] = p;
            if (i % p == 0) break;
        }
    }
    spf_initialized = true;
}

/* ---------- factorisation and parity toggle ----------
   Factors val and toggles prime parity if exponent is odd. */
static void factor_toggle(uint64_t val,
                          uint32_t *primes,
                          unsigned char *parities,
                          int *count)
{
    while (val > 1) {
        uint32_t p;
        if (val < spf_size && spf[val] != 0) {
            p = spf[val];
        } else {
            /* trial division by known primes */
            p = 0;
            for (size_t i = 0; i < prime_count; ++i) {
                uint32_t prime = prime_list[i];
                if ((uint64_t)prime * prime > val) break;
                if (val % prime == 0) { p = prime; break; }
            }
            if (!p) p = (uint32_t)val;  /* val itself is prime */
        }
        int exp = 0;
        while (val % p == 0) { val /= p; exp++; }
        if (exp & 1) toggle_prime(p, primes, parities, count);
    }
}

/* ---------- check if the triple is a Heron triangle ----------
   Uses the 4‑term product criterion. */
static bool is_heron(seq_t a, seq_t b, seq_t c)
{
    /* triangle inequality – note that the sequence is strictly increasing,
       but this guard protects against invalid predecessors. */
    if (a + b <= c) return false;

    uint64_t X = (uint64_t)a + (uint64_t)b + (uint64_t)c;
    uint64_t Y = (uint64_t)b + (uint64_t)c - (uint64_t)a;
    uint64_t Z = (uint64_t)a + (uint64_t)c - (uint64_t)b;
    uint64_t W = (uint64_t)a + (uint64_t)b - (uint64_t)c;
    /* All four values are positive integers. */

    /* Temporary arrays store the distinct primes with odd exponent. */
    static uint32_t primes_temp[MAX_PRIME_FACTORS];
    static unsigned char parities_temp[MAX_PRIME_FACTORS];
    int count = 0;

    factor_toggle(X, primes_temp, parities_temp, &count);
    factor_toggle(Y, primes_temp, parities_temp, &count);
    factor_toggle(Z, primes_temp, parities_temp, &count);
    factor_toggle(W, primes_temp, parities_temp, &count);

    return count == 0;  /* true iff the product is a perfect square. */
}

/* ---------- main sequence routine ----------
   Computes a(n) for any n ≥ 1, memoising previously generated terms. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1 – not used in tests */

    /* initialise auxiliary structures the first time */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static idx_t seq_len = 0;

    init_spf();           /* ensure prime tables exist */

    /* grow storage as needed */
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();
    }

    /* generate terms up to the requested index */
    while (seq_len < n) {
        if (seq_len == 0) {              /* a(1) = 60 */
            seq[seq_len++] = (seq_t)60;
        } else if (seq_len == 1) {       /* a(2) = 275 */
            seq[seq_len++] = (seq_t)275;
        } else if (seq_len == 2) {       /* a(3) = 325 */
            seq[seq_len++] = (seq_t)325;
        } else {
            seq_t a_prev2 = seq[seq_len - 2];
            seq_t a_prev1 = seq[seq_len - 1];

            /* search for the smallest candidate larger than a_prev1 */
            seq_t cand = a_prev1 + 1;
            seq_t limit = a_prev2 + a_prev1;  /* strict triangle inequality */

            for (; cand < limit; ++cand) {
                if (is_heron(a_prev2, a_prev1, cand)) {
                    seq[seq_len++] = cand;
                    break;
                }
            }
            /* Under normal circumstances a valid candidate is always found. */
        }

        /* expand the array if needed */
        if ((size_t)seq_len == seq_cap) {
            seq_cap <<= 1;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            if (!seq) abort();
        }
    }

    return seq[n - 1];
}
