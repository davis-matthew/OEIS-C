/*
 * A104425  Numbers n such that n, p[n], p[n]+n, p[n]-n and p[n]*n all are
 *          5-less.
 * https://oeis.org/A104425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_5_less_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th integer x such that
   x, p[x], p[x]+x, p[x]-x, p[x]*x
   are all 5‑less (contain no digit 5).
   Computed by brute‑force search. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_5_less_uint128.h"

/* External integer types supplied by the build */
extern seq_t a(idx_t n);

/* --------------------------------------------------------------
   Prime generation (cache of all primes up to a needed index)
   -------------------------------------------------------------- */
static uint64_t *prime_cache     = NULL;
static size_t    prime_cap       = 0;
static size_t    prime_size      = 0;
static uint64_t  next_prime_num  = 3;  /* next candidate to test for primality */

/* Ensure that prime_cache contains the first `idx` primes (idx >= 1).
   (prime_cache[0] = 2, prime_cache[1] = 3, ...) */
static void ensure_primes(size_t idx)
{
    if (prime_size == 0) {
        prime_cap = 16;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_cache) abort();
        prime_cache[prime_size++] = 2;          /* prime #1 */
    }

    while (prime_size < idx) {
        bool is_prime = true;
        uint64_t candidate = next_prime_num;
        for (size_t i = 0; i < prime_size; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_size == prime_cap) {
                prime_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_cache, prime_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_cache = tmp;
            }
            prime_cache[prime_size++] = candidate;
        }
        ++next_prime_num;
    }
}

/* --------------------------------------------------------------
   Main sequence function
   -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;
    static size_t  terms_cap  = 0;
    static size_t  terms_size = 0;
    static idx_t   next_candidate = 1;   /* integer to test next (1‑based) */

    /* Search for terms until we have at least n of them. */
    while (terms_size < n) {
        idx_t cand = next_candidate;        /* integer candidate */

        /* ----------------------------------------------------------------
           Check the 5‑less condition for all required values.
           All checks are performed with 128‑bit arithmetic to avoid over‑
           flow, then cast to the appropriate types where necessary.
           ---------------------------------------------------------------- */
        if (is_5_less_uint128(cand)) {
            /* Ensure we have the cand‑th prime */
            ensure_primes((size_t)cand);
            uint64_t prime = prime_cache[(size_t)cand - 1];

            if (is_5_less_uint128(prime) &&
                is_5_less_uint128((__uint128_t)prime + cand) &&
                is_5_less_uint128((__uint128_t)prime - cand) &&
                is_5_less_uint128((__uint128_t)prime * cand)) {

                /* Store the candidate as a term */
                if (terms_size == terms_cap) {
                    terms_cap = terms_cap ? terms_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    terms = tmp;
                }
                terms[terms_size++] = (seq_t)cand;
            }
        }

        ++next_candidate;
    }

    return terms[n - 1];
}
