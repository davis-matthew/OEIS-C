/*
 * A068351  Numbers n such that n*tau(n)>prime(3*n) where tau(n)=A000005(n).
 * https://oeis.org/A068351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068351: a(n) is the n-th positive integer m such that
   m * tau(m) > prime(3*m), where tau(m) is the number of divisors of m. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/build_primes_upto.h"

static const idx_t LIMIT_N          = 500000;      /* maximal n to test */
static const idx_t LIMIT_PRIME_VAL  = 30000000;    /* prime value bound */

/* Static state */
static idx_t          *spf            = NULL;
static uint32_t       *tau_arr        = NULL;
static seq_t          *primes_seq     = NULL;
static size_t         primes_cnt      = 0;
static seq_t          *terms_cache    = NULL;
static size_t         terms_len       = 0;
static size_t         terms_cap       = 0;
static idx_t          current_i       = 1;
static bool           initialized     = false;

/* Helper to perform the one–time initialization */
static void init_sequence(void)
{
    if (initialized) return;

    /* Build the smallest prime factor table up to LIMIT_N */
    build_spf(LIMIT_N, &spf);
    tau_arr = (uint32_t*)malloc((LIMIT_N + 1) * sizeof(uint32_t));
    tau_arr[1] = 1;
    for (idx_t i = 2; i <= LIMIT_N; ++i) {
        idx_t p = spf[i];
        idx_t exp = 0;
        idx_t m = i;
        while (m % p == 0) {
            ++exp;
            m /= p;
        }
        tau_arr[i] = tau_arr[m] * (exp + 1);
    }

    /* Build all primes up to LIMIT_PRIME_VAL */
    uint64_t *primes_raw = NULL;
    size_t raw_cnt = 0;
    build_primes_upto(LIMIT_PRIME_VAL, &primes_raw, &raw_cnt);
    primes_cnt = raw_cnt;
    primes_seq = (seq_t*)malloc(primes_cnt * sizeof(seq_t));
    for (size_t i = 0; i < primes_cnt; ++i) {
        primes_seq[i] = (seq_t)primes_raw[i];
    }
    free(primes_raw);

    /* Allocate initial cache for terms */
    terms_cap   = 128;
    terms_cache = (seq_t*)malloc(terms_cap * sizeof(seq_t));

    initialized = true;
}

/* Append a value to the terms cache, growing it if necessary */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        terms_cap *= 2;
        terms_cache = (seq_t*)realloc(terms_cache, terms_cap * sizeof(seq_t));
    }
    terms_cache[terms_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_sequence();

    while (terms_len < n) {
        if (current_i > LIMIT_N) {
            /* Should not happen for the required limits */
            break;
        }

        seq_t n_seq = (seq_t)current_i;
        seq_t tau_seq = (seq_t)tau_arr[current_i];
        seq_t left = n_seq * tau_seq;
        seq_t right = primes_seq[(3 * current_i) - 1]; /* primes are 1‑indexed */

        if (left > right) {
            append_term(n_seq);
        }
        ++current_i;
    }

    return terms_cache[n - 1];
}
