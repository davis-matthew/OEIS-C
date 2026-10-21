/*
 * A232447  Primes p with each odd decimal digit present at least once.
 * https://oeis.org/A232447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   build_primes_upto, digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A232447 – primes with all odd decimal digits (1,3,5,7,9) present.
   The code searches successive primes, keeping those meeting the digit
   condition, and memoises results for fast repeated access. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/digit_frequencies.h"

/* ----- static cache -----
   terms        : array of sequence terms computed so far
   terms_len     : number of stored terms
   terms_cap     : allocated capacity of terms
   prime_arr    : array of primes currently in use
   prime_cnt    : number of primes in prime_arr
   prime_idx    : next prime index to test in prime_arr
   prime_lim    : current upper bound used to build prime_arr
   last_checked : last prime that was processed (used to avoid duplicates)
----------------------------------------------------------------------- */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

static uint64_t *prime_arr = NULL;
static size_t     prime_cnt = 0;
static size_t     prime_idx = 0;
static uint64_t   prime_lim = 1000;   /* initial limit for prime generation */
static uint64_t   last_checked = 0;

/* return true if x contains all odd decimal digits 1,3,5,7,9 */
static bool has_all_odd_digits(seq_t x)
{
    unsigned int freq[10] = {0};
    digit_frequencies(x, freq);
    return freq[1] && freq[3] && freq[5] && freq[7] && freq[9];
}

/* Ensure that at least 'need' terms are available in the cache */
static void ensure_terms(idx_t need)
{
    /* Grow the terms cache if necessary */
    if (terms_cap == 0) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    while (terms_len < (size_t)need) {
        /* If we have exhausted the current prime list, expand the limit */
        if (prime_idx >= prime_cnt) {
            /* Double the prime limit */
            prime_lim *= 2;
            if (prime_arr) free(prime_arr);
            build_primes_upto((idx_t)prime_lim, &prime_arr, &prime_cnt);

            /* Skip primes that have already been processed to avoid duplicates */
            prime_idx = 0;
            while (prime_idx < prime_cnt && prime_arr[prime_idx] <= last_checked)
                ++prime_idx;
        }

        /* Scan the next prime */
        uint64_t p = prime_arr[prime_idx++];
        last_checked = p;

        if (has_all_odd_digits((seq_t)p)) {
            /* Grow terms array if needed */
            if (terms_len >= terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)p;
        }
    }
}

/* Return the n-th term of the sequence (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, but harmless */
    ensure_terms(n);
    return terms[(size_t)n - 1];
}
