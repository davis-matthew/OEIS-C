/*
 * A095411  Numbers k such that total number of decimal digits of all distinct
 *          prime factors of k is larger than the number of digits of k.
 * https://oeis.org/A095411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   uint64_digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute numbers k such that the sum of decimal digits of all
   distinct prime factors of k exceeds the number of decimal digits
   of k.  The function a(n) returns the nth such number, with
   a(1)=6 as given in the OEIS entry.  The implementation searches
   the integers in ascending order and memoises the found terms,
   so calls to a() may arrive in any order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/uint64_digit_len.h"

/* ---------- Prime list generation (single run) ---------- */
static uint32_t *prime_list = NULL;      /* array of primes up to LIMIT */
static size_t prime_count   = 0;

/* LIMIT chosen so that the 10 000th term lies well below it.
   The OEIS sequence grows roughly linearly, and the 10 000th term
   is < 10⁶.  A prime limit of 5 000 suffices, but we use 1 000 000
   to be safe. */
static void init_primes(void) {
    if (prime_list != NULL) return;  /* already initialised */

    const uint32_t LIMIT = 1000000U;
    bool *is_comp = (bool*)calloc((size_t)LIMIT + 1, sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);

    prime_list = (uint32_t*)malloc(LIMIT * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);

    prime_count = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[(size_t)i]) {
            prime_list[prime_count++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                    is_comp[(size_t)j] = true;
            }
        }
    }
    free(is_comp);
}

/* ---------- Term cache for memoisation ---------- */
static seq_t *term_cache = NULL;   /* cache of the found terms */
static size_t term_cap   = 0;
static size_t term_len   = 0;
static idx_t  next_candidate = 2;  /* start searching from 2 */

/* Ensure that term_cache contains at least n terms. The function
   looks at successive integers, tests the condition, and appends
   qualifying values to the cache. */
static void ensure_terms(size_t n) {
    init_primes();   /* primes ready on first call */

    while (term_len < n) {
        idx_t k = next_candidate++;              /* candidate integer */
        uint64_t temp = (uint64_t)k;
        int sum_digits = 0;

        /* Factorise k using the precomputed primes.  Only distinct
           prime factors contribute, so we add the digits of the
           factor once and then divide all its powers out. */
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            uint64_t pp = (uint64_t)p;
            if (pp * pp > temp) break;
            if (temp % pp == 0) {
                sum_digits += uint64_digit_len(pp);
                while (temp % pp == 0)
                    temp /= pp;
            }
        }
        /* Remaining factor (if >1) is a prime greater than sqrt(k). */
        if (temp > 1)
            sum_digits += uint64_digit_len(temp);

        int len_k = uint64_digit_len((uint64_t)k);
        if (sum_digits > len_k) {
            /* Append to cache, enlarging if necessary. */
            if (term_len == term_cap) {
                term_cap = term_cap ? term_cap * 2 : 8;
                term_cache = (seq_t*)realloc(term_cache, term_cap * sizeof(seq_t));
                if (!term_cache) exit(EXIT_FAILURE);
            }
            term_cache[term_len++] = (seq_t)k;
        }
    }
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out of bounds – safe guard */
    ensure_terms((size_t)n);
    return term_cache[(size_t)(n - 1)];
}
