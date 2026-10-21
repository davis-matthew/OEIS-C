/*
 * A088070  Numbers sandwiched between two numbers having the same number of
 *          prime divisors.
 * https://oeis.org/A088070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers m such that ω(m‑1) = ω(m+1).  Here ω(k) is the
   number of distinct prime factors of k.  The sequence starts with
   3, 4, 6, 8, … (offset 1).  The implementation caches previously
   found terms and extends the search only forward; any call
   seq_t a(idx_t n) with n≥1 returns the nth element. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/count_distinct_primes.h"

/* Cached list of computed terms. */
static seq_t *terms      = NULL;   /* array of sequence values    */
static size_t terms_count = 0;      /* number of terms stored      */
static size_t terms_cap   = 0;      /* array capacity              */

/* The largest integer m examined so far.  We start from 2 so that the
   first candidate examined is 3. */
static uint64_t last_checked = 2;

/* Append a new term, growing the array if needed. */
static void append_term(seq_t val)
{
    if (terms_count >= terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(*terms));
        if (!new_arr) abort(); /* out of memory – terminate */
        terms = new_arr;
        terms_cap = new_cap;
    }
    terms[terms_count++] = val;
}

/* Ensure that at least 'need' terms have been computed. */
static void ensure_terms(idx_t need)
{
    /* Convert need to size_t for comparison. */
    size_t target = (size_t)need;
    while (terms_count < target) {
        ++last_checked;                     /* next candidate */
        if (last_checked < 3) continue;     /* skip 0,1,2 */

        int omega_minus = count_distinct_primes((uint64_t)(last_checked - 1));
        int omega_plus  = count_distinct_primes((uint64_t)(last_checked + 1));
        if (omega_minus == omega_plus) {
            append_term((seq_t)last_checked);
        }
    }
}

/* OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* fallback for out‑of‑range index */
    ensure_terms(n);
    return terms[(size_t)n - 1];
}
