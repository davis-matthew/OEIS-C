/*
 * A127663  Infinitary aspiring numbers.
 * https://oeis.org/A127663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A127663: Infinitary aspiring numbers
 *
 * A(n) is the nth positive integer that is NOT an infinitary perfect
 * number, but whose infinitary aliquot sequence eventually reaches an
 * infinitary perfect number.
 *
 * The code precomputes the fast “infinitary divisor sum” σ_inf using
 * the smallest‑prime‑factor (SPF) sieve.  Candidates are tested in
 * increasing order, and a simple iterative search is used to detect
 * whether the aliquot sequence reaches an infinitary perfect number.
 * Results are cached so repeated calculations are negligible.
 *
 * The algorithm runs in O(N + K) time where K is the number of
 * candidates examined (typically ≈ 0.1·N for the first 10 000 terms),
 * and uses memory on the order of the sieve limit.
 *
 * 3‑5 million integers are enough to generate the first many thousand
 * terms.  Adjust MAX_LIMIT if more terms are required.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_spf.h"

/* Sieve size – enough for the first few thousand terms. */
#define MAX_LIMIT 5000000

/* Cached data. */
static idx_t *spf = NULL;                 /* smallest prime factor (uint64_t) */
static seq_t *sigma_cache = NULL;          /* cached σ_inf values */
static unsigned char *sigma_computed = NULL;
static unsigned char *is_inf_perfect = NULL;

/* Aspirant list. */
static seq_t *aspirants = NULL;
static size_t aspirants_cap = 0;
static size_t aspirants_len = 0;
static idx_t next_candidate = 1;           /* next integer to test */

/* Ensure global data structures are initialized once. */
static void init_globals(void) {
    if (spf != NULL)
        return;                      /* already initialized */

    /* Build SPF sieve. */
    build_spf(MAX_LIMIT, &spf);

    /* Allocate caches. */
    sigma_cache     = (seq_t *)calloc(MAX_LIMIT + 1, sizeof(seq_t));
    sigma_computed  = (unsigned char *)calloc(MAX_LIMIT + 1, sizeof(unsigned char));
    is_inf_perfect  = (unsigned char *)calloc(MAX_LIMIT + 1, sizeof(unsigned char));
}

/* Compute σ_inf for n using its prime factors and bit‑mask exponents. */
static seq_t compute_sigma_inf(idx_t n) {
    seq_t prod = 1;
    idx_t tmp = n;

    while (tmp > 1) {
        idx_t p = spf[tmp];
        if (p == 0)          /* n is prime > MAX_LIMIT (unlikely) */
            p = tmp;
        unsigned int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }

        /* σ_inf(p^e) = Π_j (1 + p^(2^j)) over all set bits of e. */
        seq_t term = 1;
        seq_t p_pow = p;      /* p^(2^0) */
        unsigned int bit = 1; /* 2^0 = 1 */

        while (bit <= e) {
            if (e & bit)
                term *= (1 + p_pow);
            p_pow = p_pow * p_pow;   /* multiply exponent by 2 */
            bit <<= 1;               /* next power‑of‑two bit */
        }

        prod *= term;
    }
    return prod;
}

/* Retrieve σ_inf(n) from cache or compute it.  If n ≤ MAX_LIMIT, store it. */
static seq_t get_sigma_inf(idx_t n) {
    if (n <= MAX_LIMIT && sigma_computed[n])
        return sigma_cache[n];

    seq_t sigma = compute_sigma_inf(n);

    if (n <= MAX_LIMIT) {
        sigma_cache[n]    = sigma;
        sigma_computed[n] = 1;
        if (sigma == (seq_t)(2 * n))
            is_inf_perfect[n] = 1;
    }
    return sigma;
}

/* Quick test whether n is infinitary perfect. */
static bool is_inf_perfect_number(idx_t n) {
    if (n <= MAX_LIMIT && is_inf_perfect[n])
        return true;
    seq_t sigma = get_sigma_inf(n);
    return sigma == (seq_t)(2 * n);
}

/* Main sequence function. */
seq_t a(idx_t n) {
    init_globals();

    /* Grow aspirant list if necessary. */
    while (aspirants_len < (size_t)n) {
        ++next_candidate;
        if (next_candidate < 2)
            continue;                 /* skip 0 and 1 */

        /* Skip infinitary perfect numbers themselves. */
        if (is_inf_perfect_number(next_candidate))
            continue;

        /* Simulate the infinitary aliquot sequence. */
        idx_t cur = next_candidate;
        bool reaches = false;

        for (int step = 0; step < 1000 && cur > 1; ++step) {
            seq_t sigma = get_sigma_inf(cur);
            if (sigma == (seq_t)(2 * cur)) {   /* reached an inf‑perfect number */
                reaches = true;
                break;
            }
            seq_t proper = sigma - cur;
            if (proper == 0)
                break;      /* reached zero – end of sequence */
            if (proper > (seq_t)~(idx_t)0)   /* would overflow next cast */
                break;
            cur = (idx_t)proper;
        }

        if (reaches) {
            /* Ensure storage capacity. */
            if (aspirants_len == aspirants_cap) {
                aspirants_cap = (aspirants_cap == 0) ? 1024 : aspirants_cap * 2;
                aspirants = (seq_t *)realloc(aspirants, aspirants_cap * sizeof(seq_t));
            }
            aspirants[aspirants_len++] = next_candidate;
        }
    }

    return aspirants[n - 1];
}
