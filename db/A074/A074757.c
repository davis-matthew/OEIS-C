/*
 * A074757  Numbers n such that tau(n) = (tau(n+1)+tau(n-1))/2.
 * https://oeis.org/A074757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   estimate_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074757: numbers n for which τ(n) = (τ(n-1)+τ(n+1))/2.
 * The implementation builds the divisor‑count sieve on demand and
 * records all terms found up to the requested index.  The algorithm
 * is O(N log N) in the final search limits and works for any of the
 * supported integer types. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/estimate_limit.h"

/* Static storage for the divisor counts and the resulting sequence */
static uint32_t *tau      = NULL;          /* divisor counts, size = limit+2 */
static size_t   tau_sz   = 0;              /* allocated size of tau */
static seq_t   *seq      = NULL;          /* values of the sequence */
static size_t   seq_sz   = 0;              /* number of elements stored */
static size_t   seq_cap  = 0;              /* allocated capacity of seq */
static idx_t    current_limit = 0;         /* maximum n checked in the current sieve */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Rebuild the sieve of divisor counts up to at least 'limit'.
 * The array is cleared and recomputed entirely. */
static void recompute_sieve(idx_t limit)
{
    /* Allocate or resize the tau array. */
    if (tau_sz < (size_t)(limit + 2)) {
        free(tau);
        tau = (uint32_t *)calloc((size_t)(limit + 2), sizeof(uint32_t));
        tau_sz = (size_t)(limit + 2);
    } else {
        /* Zero the relevant part. */
        for (size_t i = 0; i < (size_t)(limit + 2); ++i) tau[i] = 0;
    }

    /* Classic divisor-sieve: add 1 to each multiple of i. */
    for (idx_t i = 1; i <= limit + 1; ++i) {
        for (idx_t j = i; j <= limit + 1; j += i) {
            ++tau[j];
        }
    }
}

/* ------------------------------------------------------------------ */
/* Ensure that the sequence has at least n_needed terms available.
 * We recompute the sieve and rebuild the sequence if required. */
static void ensure_sequence(idx_t n_needed)
{
    if (seq_sz >= n_needed) return; /* already satisfied */

    /* Determine an initial limit and grow until enough terms are found. */
    idx_t limit = estimate_limit(n_needed);
    while (1) {
        recompute_sieve(limit);

        /* Reset current sequence data. */
        seq_sz = 0;

        /* Ensure the sequence array can hold all candidates. */
        if (seq_cap < (size_t)(limit + 2)) {
            seq_cap = (size_t)(limit + 2);
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }

        /* Scan integers from 2 upwards; τ(0) is undefined so we skip 1. */
        for (idx_t x = 2; x <= limit; ++x) {
            uint32_t left  = tau[x - 1];
            uint32_t right = tau[x + 1];
            if ((left + right) & 1U) continue;        /* sum must be even */
            uint32_t rhs = (left + right) >> 1;       /* (left+right)/2 */
            if (tau[x] == rhs) {
                seq[seq_sz++] = (seq_t)x;
                if (seq_sz >= n_needed) break;
            }
        }

        if (seq_sz >= n_needed) break; /* success */

        /* Not enough terms – increase the search window and try again. */
        limit *= 2;
    }

    current_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Public entry point required by the build system. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0; /* undefined for n == 0 */
    ensure_sequence(n);
    return seq[n - 1];
}
