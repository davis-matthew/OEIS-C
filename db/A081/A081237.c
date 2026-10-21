/*
 * A081237  Numbers n with no k, 0<k<n, such that mu(n-k)=mu(n)=mu(n+k), where
 *          mu=A008683 (Moebius function).
 * https://oeis.org/A081237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that for no 0<k<n does
   mu(n-k) = mu(n) = mu(n+k).
   (mu = Möbius function) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/* ------------------------------------------------------------------ */
/*  Pre‑compute the Möbius function up to a fixed maximum.            */
/* ------------------------------------------------------------------ */

#define MU_HALF_LIMIT 1000000  /* limit for candidate n            */
#define MU_ARRAY_SIZE (2*MU_HALF_LIMIT+1)  /* need mu up to 2*n-1 */

/* mu[i] will hold the Möbius value for i>=1. 0 is unused.              */
static int mu[MU_ARRAY_SIZE];
static bool comp[MU_ARRAY_SIZE];
static int  primes[200000];        /* enough for 2*MU_HALF_LIMIT       */
static size_t primes_cnt = 0;
static bool mu_inited = false;

static void init_mu(void)
{
    if (mu_inited) return;
    mu[1] = 1;
    comp[0] = comp[1] = true;     /* just to silence unused warning */

    for (size_t i = 2; i < MU_ARRAY_SIZE; ++i) {
        if (!comp[i]) {
            primes[primes_cnt++] = (int)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            int p = primes[j];
            size_t v = i * (size_t)p;
            if (v >= MU_ARRAY_SIZE) break;
            comp[v] = true;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }
    mu_inited = true;
}

/* ------------------------------------------------------------------ */
/*  Storage for sequence terms.                                        */
/* ------------------------------------------------------------------ */

/* We keep enough room for a few hundred terms; the sequence is expected
   to be very sparse (only 8 known terms).  Using a static array
   avoids any dynamic allocation that would break for an arbitrary‑
   precision seq_t. */
#define MAX_TERMS 10000

static seq_t terms[MAX_TERMS];
static size_t terms_len = 0;          /* number of terms stored      */
static idx_t  next_candidate = 1;     /* next n to test              */
static bool finished = false;         /* no more terms within limits */

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                            */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (!mu_inited) init_mu();

    /* Lazy initialization: nothing needed beyond static array. */

    /* Generate terms until we have at least n of them or until we run
       out of pre‑computed Möbius values. */
    while (terms_len < (size_t)n && !finished) {
        /* Search for the next valid number. */
        bool found = false;
        while (next_candidate < MU_HALF_LIMIT) {
            /* We need mu values up to 2*next_candidate-1. */
            if (2*next_candidate - 1 >= MU_ARRAY_SIZE) {
                finished = true;
                break;
            }

            int mu_n = mu[(size_t)next_candidate];
            bool ok = true;
            /* Check all k in 1..next_candidate-1. */
            for (idx_t k = 1; k < next_candidate; ++k) {
                int mu_left  = mu[(size_t)(next_candidate - k)];
                int mu_right = mu[(size_t)(next_candidate + k)];
                if (mu_left == mu_n && mu_right == mu_n) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                /* Store this term. */
                assert(terms_len < MAX_TERMS);
                terms[terms_len++] = (seq_t)next_candidate;
                next_candidate++;   /* next candidate for future calls */
                found = true;
                break;
            }
            ++next_candidate;
        }

        if (!found) {
            /* Either we reached the limit or no more terms can be found. */
            finished = true;
        }
    }

    if (terms_len >= (size_t)n) {
        return terms[n - 1];
    }
    /* No known term at this index.  Return 0 as specified. */
    return (seq_t)0;
}
