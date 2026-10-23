/*
 * A166070  Sorted sequence of primes and multiply perfect numbers.
 * https://oeis.org/A166070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_multiply_perfect
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all positive integers up to a fixed limit, test whether each
   is a prime or a multiply‑perfect number (including 1), and store the results
   in a sorted list. a(n) returns the n-th element of this list. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_multiply_perfect.h"

seq_t a(idx_t n)
{
    /* -------------------------------------------------------------
       static state for lazy one‑time initialisation
       ------------------------------------------------------------- */
    static bool      initialized = false;
    static seq_t    *terms       = NULL;
    static size_t    terms_cnt   = 0;
    static size_t    terms_cap   = 0;
    static uint32_t *spf         = NULL;   /* smallest prime factor table */

    if (!initialized) {
        const idx_t LIMIT = 200000;   /* enough for n ≤ 10 000 */
        /* ---------- 1. build SPF sieve up to LIMIT ---------- */
        spf = (uint32_t *)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
        if (!spf) abort();   /* out of memory */

        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (spf[i] == 0) {          /* prime */
                spf[i] = (uint32_t)i;
                if (i <= LIMIT / i) {
                    for (idx_t j = i * i; j <= LIMIT; j += i) {
                        if (spf[j] == 0) spf[j] = (uint32_t)i;
                    }
                }
            }
        }

        /* ---------- 2. allocate space for the sequence ---------- */
        terms_cap = 5000;
        terms     = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();

        /* ---------- 3. compute all terms up to LIMIT ---------- */
        for (idx_t k = 1; k <= LIMIT; ++k) {
            bool add = false;
            if (k == 1) add = true;
            else if (spf[k] == k) add = true;          /* prime */
            else if (is_multiply_perfect(k, spf)) add = true;

            if (add) {
                if (terms_cnt >= terms_cap) {
                    terms_cap <<= 1;
                    terms = (seq_t *)realloc(terms,
                            terms_cap * sizeof(seq_t));
                    if (!terms) abort();
                }
                terms[terms_cnt++] = (seq_t)k;
            }
        }
        /* ---------- 4. free SPF table ---------- */
        free(spf);
        spf = NULL;

        initialized = true;
    }

    if (n == 0) return 0;         /* undefined for n<1, return 0 safely */
    if ((idx_t)(n - 1) < terms_cnt) return terms[n - 1];
    return 0;                     /* should never happen for n ≤ 10 000 */
}
