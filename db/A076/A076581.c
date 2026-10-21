/*
 * A076581  Triangular numbers which are 7-almost primes.
 * https://oeis.org/A076581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 23, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th triangular number that is a 7‑almost prime
 *        (has exactly seven prime factors, counted with multiplicity).
 *        T(k) = k(k+1)/2.  Offset: 1.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t processed_limit = 0;      /* largest k processed so far            */
static size_t terms_len = 0;          /* number of terms stored                */
static size_t terms_cap = 0;          /* capacity of the terms array           */
static seq_t *terms = NULL;           /* stored triangular numbers             */

static idx_t *spf = NULL;             /* smallest prime factor (indices 0..max_k+1) */
static uint8_t *omega = NULL;         /* number of prime factors (with multiplicity) */

/* Rebuild the spf array and the omega array up to k = max_k.
   The arrays are sized so that index max_k+1 is valid. */
static void rebuild_spf_omega_to(idx_t max_k)
{
    idx_t size = max_k + 2;                       /* indices 0 .. max_k+1               */
    idx_t *new_spf = (idx_t *)calloc((size_t)size, sizeof(idx_t));
    if (!new_spf) exit(EXIT_FAILURE);

    new_spf[0] = 0;
    new_spf[1] = 1;

    /* Sieve to fill smallest prime factor */
    for (idx_t i = 2; i <= max_k + 1; ++i) {
        if (new_spf[i] == 0) {                    /* i is prime                      */
            new_spf[i] = i;
            if (i <= (max_k + 1) / i) {          /* avoid overflow in i*i           */
                for (idx_t j = i * i; j <= max_k + 1; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = i;
            }
        }
    }

    /* Compute omega (number of prime factors counted with multiplicity) */
    uint8_t *new_omega = (uint8_t *)calloc((size_t)size, sizeof(uint8_t));
    if (!new_omega) exit(EXIT_FAILURE);
    new_omega[0] = new_omega[1] = 0;
    for (idx_t i = 2; i < size; ++i) {
        idx_t div = i / new_spf[i];
        new_omega[i] = new_omega[div] + 1;
    }

    /* Replace old arrays */
    free(spf);
    free(omega);
    spf = new_spf;
    omega = new_omega;
}

/* Append a triangular number to the global list. */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 1024;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, harmless */

    while (terms_len < (size_t)n) {
        /* Expand the search range.  Start with 2000, double each time. */
        idx_t new_limit = (processed_limit == 0) ? 2000 : processed_limit * 2;

        /* Build/extend the spf and omega tables to cover k = new_limit */
        rebuild_spf_omega_to(new_limit);

        /* Scan new k values and collect qualifying triangular numbers. */
        for (idx_t k = processed_limit + 1; k <= new_limit; ++k) {
            uint8_t sumOmega = omega[k] + omega[k + 1] - 1;
            if (sumOmega == 7) {
                seq_t tri;
                if (k & 1) {
                    tri = (seq_t)k * ((seq_t)((k + 1) >> 1));
                } else {
                    tri = ((seq_t)(k >> 1)) * (seq_t)(k + 1);
                }
                append_term(tri);
            }
        }

        processed_limit = new_limit;
    }

    return terms[n - 1];
}
