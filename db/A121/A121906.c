/*
 * A121906  Excess of n-th 3-almost prime A014612 over previous prime.
 * https://oeis.org/A121906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min{A014612(n) – p | p prime, 1 < p < A014612(n)}
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a sieve of smallest prime factors (SPF) and
   scans the natural numbers to collect 3‑almost primes.  The differences
   between each such prime and its immediate predecessor prime are
   stored in a dynamic array.  The array is resized as needed to
   guarantee that the nth value can be returned in O(1) after the
   first generation.  All allocations are done in a way that is valid
   both in C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>           /* for size_t */
#include "../utils/build_spf.h"    /* void build_spf(idx_t limit, idx_t **spf_out); */

#if defined(__cplusplus)
/* In C++ we use new/delete for seq_t arrays. */
#define ALLOC_SEQ(cap)        new seq_t[(cap)]
#define FREE_SEQ(p)           delete[] (p)
#else
/* In C we use calloc/free. */
#define ALLOC_SEQ(cap)        ((seq_t *)calloc((cap), sizeof(seq_t)))
#define FREE_SEQ(p)           free(p)
#endif

/* --------------------------------------------------------------------- */
/* State used to cache the sieve, SPF table, and the differences.      */
/* --------------------------------------------------------------------- */
typedef struct {
    idx_t  limit;          /* current sieve limit */
    idx_t *spf;            /* smallest prime factor array */
    seq_t *diffs;          /* differences a(n) */
    idx_t diff_cnt;        /* number of differences stored */
    bool   initialized;    /* true after first allocation */
} State;

static State state = {0, NULL, NULL, 0, false};

/* --------------------------------------------------------------------- */
/* Helper: estimate a sieve limit that should contain at least n         */
/* 3‑almost primes.  The factor 20 is empirical and is far on the safe  */
/* side.                                                                  */
/* --------------------------------------------------------------------- */
static idx_t estimate_limit(idx_t n)
{
    return n * 20 + 100;
}

/* --------------------------------------------------------------------- */
/* Recompute the sieve and the differences up to new_limit.      */
/* All previous arrays are freed and replaced.                         */
/* --------------------------------------------------------------------- */
static void recompute(idx_t new_limit)
{
    /* Free old data if any */
    if (state.initialized) {
        FREE_SEQ(state.diffs);
        free(state.spf);
    }

    /* Build SPF table up to new_limit */
    build_spf(new_limit, &state.spf);

    /* Allocate array for differences – worst case one per number */
    state.diffs = ALLOC_SEQ(new_limit + 1);
    state.diff_cnt = 0;

    /* Auxiliary array to store ω(n) (number of prime factors with multiplicity) */
    uint8_t *omega = (uint8_t *)calloc((size_t)(new_limit + 1), sizeof(uint8_t));
    if (!omega) {
        /* Out of memory – fall back to empty state */
        state.diff_cnt = 0;
        return;
    }
    omega[0] = omega[1] = 0;

    idx_t last_prime = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        /* Update last_prime if i is prime (spf[i] == i). */
        if (state.spf[i] == i) {
            last_prime = i;
        }

        /* ω(i) = ω(i / spf[i]) + 1. */
        omega[i] = omega[i / state.spf[i]] + 1;

        if (omega[i] == 3) {
            /* Record the excess over the previous prime */
            state.diffs[state.diff_cnt++] = (seq_t)(i - last_prime);
        }
    }

    free(omega);
    state.limit       = new_limit;
    state.initialized = true;
}

/* --------------------------------------------------------------------- */
/* Ensure that at least n differences have been computed.  If not, the  */
/* sieve limit is increased until the condition is satisfied.             */
/* --------------------------------------------------------------------- */
static void ensure_at_least(idx_t n)
{
    if (state.initialized && state.diff_cnt >= n)
        return;

    idx_t new_limit;
    if (!state.initialized) {
        new_limit = estimate_limit(n);
    } else {
        new_limit = state.limit;
        if (new_limit < estimate_limit(n))
            new_limit = estimate_limit(n);
    }

    while (state.diff_cnt < n) {
        recompute(new_limit);
        if (state.diff_cnt >= n)
            break;
        /* Double the sieve size if we still lack enough entries */
        new_limit *= 2;
    }
}

/* --------------------------------------------------------------------- */
/* Public function – external linkage as required.                     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for offset < 1, return 0 */
        return (seq_t)0;
    ensure_at_least(n);
    return state.diffs[n - 1];
}
