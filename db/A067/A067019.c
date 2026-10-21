/*
 * A067019  Odd numbers with an odd number of prime factors (counted with
 *          multiplicity).
 * https://oeis.org/A067019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067019 : odd numbers with an odd number of prime factors
   (counted with multiplicity).  This implementation generates terms
   lazily, using a dynamically growing sieve of smallest prime factors
   and stores all previously found terms so that calls with arbitrary
   n are answered in O(1) after the initial generating loop. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global data structures */
static uint32_t *spf = NULL;      /* smallest prime factor table       */
static size_t   spf_limit = 0;    /* current highest index in spf      */

static seq_t *terms = NULL;      /* array of computed terms           */
static size_t  terms_len = 0;    /* number of terms stored            */
static size_t  terms_cap = 0;    /* capacity of terms array           */

static uint64_t last_checked = 1;/* last odd integer processed        */

/* ------------------------------------------------------------------ */
/* Expand the sieve of smallest prime factors up to at least 'need'.
   The sieve is rebuilt from scratch whenever it grows.           */
static void ensure_spf(size_t need)
{
    if (need <= spf_limit)
        return;

    size_t new_limit = spf_limit ? spf_limit : 16;
    while (new_limit < need)
        new_limit <<= 1;                /* double until enough          */

    uint32_t *new_spf = (uint32_t*)calloc(new_limit + 1, sizeof(uint32_t));
    for (size_t p = 2; p <= new_limit; ++p) {
        if (new_spf[p] == 0) {           /* p is prime                  */
            new_spf[p] = (uint32_t)p;
            if ((uint64_t)p * p <= new_limit) {
                for (size_t m = p * p; m <= new_limit; m += p)
                    if (new_spf[m] == 0)
                        new_spf[m] = (uint32_t)p;
            }
        }
    }
    if (spf)
        free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/* Append a newly found term to the global array, growing it if needed. */
static void append_term(seq_t value)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap << 1 : 16;
        terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = value;
}

/* ------------------------------------------------------------------ */
/* Compute the n-th term of the sequence.  n >= 1.                      */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n terms. */
    while (terms_len < (size_t)n) {
        /* Next odd candidate */
        uint64_t next = last_checked + 2;

        /* Ensure the sieve can factor 'next' */
        ensure_spf((size_t)(next + 1));

        /* Determine parity of Ω(next) */
        size_t temp = (size_t)next;
        int parity = 0;
        while (temp > 1) {
            uint32_t p = spf[temp];
            parity ^= 1;
            temp /= p;
        }

        /* Store if Ω(next) is odd */
        if (parity == 1)
            append_term((seq_t)next);

        last_checked = next;
    }

    return terms[(size_t)n - 1];
}
