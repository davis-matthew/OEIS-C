/*
 * A124968  Numbers which can be expressed as the ordered sum of 3 squares in
 *          4 or more different ways.
 * https://oeis.org/A124968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 9826, u32 n <= 9826, u64 n <= 9826,
 *            u128 n <= 9826, big any n
 * Helpers:   vec_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A124968: Numbers which can be expressed as the ordered sum of 3 squares in 4 or more different ways.  
    The function returns the n-th such number (a(1)=81).  The implementation
    follows a straightforward enumerative sieve: all unordered triples
    (a,b,c) with a≤b≤c are generated, the sums a²+b²+c² are counted,
    and the numbers with at least 4 representations are stored.  The
    sieve is computed lazily on first use, and automatically expanded
    if an index larger than precomputed terms is requested.  This
    ensures correct behaviour for any n>=1. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/vec_zero.h"

/* --------- internal helpers (static) ---------------- */

/* Count array element type: 16‑bit is sufficient for the
   small counts that arise for n ≤ 2^31. */
typedef uint16_t rep_t;

/* Global state --------------------------------------------------- */
static rep_t *rep_count = NULL;   /* representation counts */
static idx_t  rep_size  = 0;      /* size of rep_count (rep_size-1 is current max n) */

static seq_t *terms    = NULL;    /* list of sequence terms */
static size_t terms_used = 0;     /* number of terms stored */
static size_t terms_cap  = 0;     /* allocated capacity for terms */

#define INITIAL_LIMIT 300000   /* initial sieve limit */

/* Recompute the sieve up to a new limit, rebuilding rep_count and terms. */
static void recompute(idx_t new_limit) {
    /* free old data */
    free(rep_count);
    free(terms);
    rep_count = NULL;
    terms = NULL;
    terms_used = 0;
    terms_cap  = 0;

    /* allocate and compute representation counts */
    rep_size = new_limit + 1;
    rep_count = (rep_t *)calloc(rep_size, sizeof(rep_t));   /* zeroed */

    /* Enumerate unordered triples a ≤ b ≤ c with a² + b² + c² ≤ new_limit */
    for (unsigned a = 0; (unsigned long long)a * a <= new_limit; ++a) {
        unsigned long long a2 = (unsigned long long)a * a;
        for (unsigned b = a; a2 + (unsigned long long)b * b <= new_limit; ++b) {
            unsigned long long b2 = (unsigned long long)b * b;
            unsigned long long ab = a2 + b2;
            for (unsigned c = b; ab + (unsigned long long)c * c <= new_limit; ++c) {
                unsigned long long n = ab + (unsigned long long)c * c;
                ++rep_count[(idx_t)n];
            }
        }
    }

    /* Collect numbers that have at least 4 representations */
    for (idx_t i = 1; i < rep_size; ++i) {
        if (rep_count[i] >= 4) {
            if (terms_used == terms_cap) {
                terms_cap = terms_cap == 0 ? 16 : terms_cap << 1;
                terms = (seq_t *)realloc(terms,
                                         terms_cap * sizeof(seq_t));
            }
            terms[terms_used++] = (seq_t)i;
        }
    }
}

/* Ensure that the sequence is initialised and that it contains at
   least n terms.  If necessary, the sieve limit is increased. */
static void ensure(idx_t n) {
    if (!rep_count) {
        /* first time: initialise with a reasonable limit */
        recompute(INITIAL_LIMIT);
    }
    while (n - 1 >= terms_used) {
        /* not enough terms yet: double the sieve limit */
        idx_t current_limit = rep_size - 1;
        idx_t new_limit = current_limit ? current_limit << 1 : 1;
        recompute(new_limit);
    }
}

/* --------- public API ------------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* out of bounds, defined as 0 */
    ensure(n);
    return terms[n - 1];
}
