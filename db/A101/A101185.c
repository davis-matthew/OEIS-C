/*
 * A101185  Complement of A101184.
 * https://oeis.org/A101185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complement of A101184: 
   a(n) is the nth positive integer that cannot be written as 
   n + π(n) + π(π(n)) + …  where π is the prime‑counting function.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static const size_t LIMIT = 300000;   /* sufficient for at least 20000 missing terms */

/* Build the complement list up to LIMIT. */
static void build_missing(seq_t **out_vals, idx_t *out_len)
{
    /* 1. sieve of Eratosthenes for primality */
    uint8_t *is_prime = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    for (size_t i = 2; i <= LIMIT; ++i) is_prime[i] = 1;
    for (size_t p = 2; p * p <= LIMIT; ++p)
        if (is_prime[p])
            for (size_t j = p * p; j <= LIMIT; j += p)
                is_prime[j] = 0;

    /* 2. prefix count of primes (π) */
    idx_t *pi = (idx_t *)calloc(LIMIT + 1, sizeof(idx_t));
    pi[0] = 0;
    for (size_t i = 1; i <= LIMIT; ++i)
        pi[i] = pi[i - 1] + (idx_t)is_prime[i];

    /* 3. mark every representable value v = n + π(n) + π(π(n)) + …  */
    uint8_t *is_repr = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    for (size_t n = 1; n <= LIMIT; ++n) {
        seq_t v = (seq_t)n;
        idx_t t = (idx_t)n;
        while (t > 1) {
            t = pi[t];
            v += t;
        }
        if (v <= LIMIT)
            is_repr[(size_t)v] = 1;
    }

    /* 4. collect the complement */
    size_t cap = 20000;
    seq_t *missing = (seq_t *)malloc(cap * sizeof(seq_t));
    size_t missing_len = 0;
    for (size_t i = 1; i <= LIMIT; ++i) {
        if (!is_repr[i]) {
            if (missing_len == cap) {
                cap <<= 1;
                missing = (seq_t *)realloc(missing, cap * sizeof(seq_t));
            }
            missing[missing_len++] = (seq_t)i;
        }
    }

    free(is_prime);
    free(pi);
    free(is_repr);

    *out_vals = missing;
    *out_len = (idx_t)missing_len;
}

/* static storage of the complement list */
static seq_t *missing_vals = NULL;
static idx_t missing_cnt = 0;
static bool initialized = false;

/* required function */
seq_t a(idx_t n)
{
    if (!initialized) {
        build_missing(&missing_vals, &missing_cnt);
        initialized = true;
    }
    if (n == 0 || n > missing_cnt)
        return 0;  /* undefined for out‑of‑range queries */
    return missing_vals[n - 1];
}
