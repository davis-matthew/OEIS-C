/*
 * A234286  Positive odd numbers n such that 2m - sigma(m) is never equal to
 *          n, where sigma(.) is the sum of divisors function A000203.
 *          Conjectural.
 * https://oeis.org/A234286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A234286:  positive odd n such that 2m - σ(m) ≠ n for every m.
 *  The function builds a list of all such odd numbers using a divisor‑sum
 *  sieve up to a fixed bound, and returns the n‑th term on demand.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*      Configuration                                                      */
/* --------------------------------------------------------------------- */
static const idx_t MAX_M = 20000000ULL;      /* upper bound for m (≈2×10⁷) */

/* --------------------------------------------------------------------- */
/*      Static data structures                                            */
/* --------------------------------------------------------------------- */
static uint32_t *sigma_arr = NULL;           /* divisor sums up to MAX_M    */
static unsigned char *odd_f_present = NULL; /* whether an odd f value occurs  */
static seq_t *seq_terms = NULL;              /* stored terms of the sequence */
static size_t terms_cap = 0;                 /* current capacity of seq_terms */
static idx_t terms_cnt = 0;                  /* number of terms stored */
static idx_t next_odd_to_test = 3;           /* next odd number to test      */
static bool initialized = false;

/* --------------------------------------------------------------------- */
/*      Forward declaration of local helper                             */
/* --------------------------------------------------------------------- */
static void init_sequence(void);

/* --------------------------------------------------------------------- */
/*      Initialise static data                                           */
/* --------------------------------------------------------------------- */
static void init_sequence(void)
{
    if (initialized) return;

    /* Allocate and compute σ(m) via a classical divisor‑sieve. */
    sigma_arr = (uint32_t*)calloc((size_t)(MAX_M + 1), sizeof(uint32_t));
    if (!sigma_arr) exit(EXIT_FAILURE);
    for (idx_t i = 1; i <= MAX_M; ++i) {
        for (idx_t j = i; j <= MAX_M; j += i) {
            sigma_arr[j] += (uint32_t)i;
        }
    }

    /* Mark odd values that occur as 2m – σ(m). */
    odd_f_present = (unsigned char*)calloc((size_t)(MAX_M + 1),
                                            sizeof(unsigned char));
    if (!odd_f_present) exit(EXIT_FAILURE);
    for (idx_t m = 1; m <= MAX_M; ++m) {
        int64_t f = (int64_t)2 * m - (int64_t)sigma_arr[m];
        if (f > 0 && (f & 1) && f <= (int64_t)MAX_M) {
            odd_f_present[(size_t)f] = 1;
        }
    }

    /* Allocate space for the sequence terms. */
    terms_cap = 100000;                     /* initial capacity */
    seq_terms = (seq_t*)calloc(terms_cap, sizeof(seq_t));
    if (!seq_terms) exit(EXIT_FAILURE);

    /* Collect terms up to the capacity or until odd limit runs out. */
    idx_t odd = 3;
    while (terms_cnt < terms_cap) {
        if (odd > MAX_M) break;             /* safety guard */
        if (!odd_f_present[(size_t)odd]) {
            if (terms_cnt >= terms_cap) {   /* grow array if needed */
                terms_cap <<= 1;
                seq_terms = (seq_t*)realloc(seq_terms,
                                             terms_cap * sizeof(seq_t));
                if (!seq_terms) exit(EXIT_FAILURE);
            }
            seq_terms[terms_cnt++] = (seq_t)odd;
        }
        odd += 2;
    }
    next_odd_to_test = odd;                /* next odd to be tested later */
    initialized = true;
}

/* --------------------------------------------------------------------- */
/*      Grow the pre‑computed list until it contains at least `target`
 *      terms.                                                     */
/* --------------------------------------------------------------------- */
static void grow_terms(idx_t target)
{
    /* Ensure seq_terms has space for at least target elements. */
    while (terms_cnt < target) {
        if (next_odd_to_test > MAX_M) {
            /* Out of pre‑computed range – the current bound is assumed
               sufficient for all requested indices (per problem
               constraints). */
            break;
        }
        if (!odd_f_present[(size_t)next_odd_to_test]) {
            if (terms_cnt >= terms_cap) {
                terms_cap <<= 1;
                seq_terms = (seq_t*)realloc(seq_terms,
                                             terms_cap * sizeof(seq_t));
                if (!seq_terms) exit(EXIT_FAILURE);
            }
            seq_terms[terms_cnt++] = (seq_t)next_odd_to_test;
        }
        next_odd_to_test += 2;
    }
}

/* --------------------------------------------------------------------- */
/*      Public function – OEIS sequence entry                          */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but guard against misuse */
    init_sequence();                /* one‑time initialization */

    /* Grow the pre‑computed list until it contains the nth term. */
    if (n > terms_cnt)
        grow_terms(n);

    /* index 1 corresponds to the first term in seq_terms */
    return seq_terms[n - 1];
}
