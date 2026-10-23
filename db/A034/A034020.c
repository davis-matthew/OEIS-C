/*
 * A034020  Not of the form x^2 + x*y + y^2.
 * https://oeis.org/A034020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer not representable as * x^2 +
 *            x*y + y^2 . * Criterion: an integer m is representable iff every
 *            prime p ≡ 2 (mod 3) * appears with an even exponent in its prime
 *            factorisation. Hence * m is a term of the sequence exactly when
 *            some such prime has odd * exponent. We ge...
 * Range:     u8 n <= 174, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>   /* for sqrt */

static uint64_t *spf = NULL;          /* smallest prime factor array   */
static size_t   spf_limit = 0;        /* current maximum of spf        */

static seq_t *terms = NULL;           /* array of computed terms       */
static size_t terms_len = 0, terms_cap = 0;

static uint64_t next_candidate = 1;   /* last inspected number         */

/* Initialise the SPF sieve up to a fixed limit that covers
 * n ≈ 10 000 (the sequence grows roughly linearly). */
static void init_spf(void)
{
    const size_t LIMIT = 200000;           /* safe for n ≤ 10 000         */
    spf = (uint64_t *)malloc((LIMIT + 1) * sizeof(uint64_t));
    if (!spf) return;
    for (size_t i = 0; i <= LIMIT; ++i) spf[i] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            for (size_t j = i * i; j <= LIMIT; j += i)
                if (!spf[j]) spf[j] = i;
        }
    }
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!spf[i]) spf[i] = i;
    spf_limit = LIMIT;
}

/* Check whether an unsigned integer m can be written as
 * x^2 + x*y + y^2 using the prime‑factor criterion. */
static bool is_nonrepresentable(uint64_t m)
{
    while (m > 1) {
        uint64_t p = spf[m];
        unsigned int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if ((p % 3) == 2 && (exp & 1))   /* odd exponent of a 2 mod 3 prime */
            return true;
    }
    return false;
}

/* Append a new term to the global array. */
static void append_term(uint64_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_terms) return;          /* out of memory – silent failure */
        terms = new_terms;
        terms_cap = new_cap;
    }
    terms[terms_len++] = (seq_t)val;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) { init_spf(); initialized = true; }

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        ++next_candidate;                 /* consider successive integers */
        if (next_candidate <= 1) continue; /* skip non‑positive values */

        if (is_nonrepresentable(next_candidate))
            append_term(next_candidate);
    }
    return terms[n-1];    /* 1‑based indexing for OEIS */
}
