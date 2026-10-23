/*
 * A023196  Nondeficient numbers: numbers k such that sigma(k) >= 2k; union of
 *          A000396 and A005101.
 * https://oeis.org/A023196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th nondeficient number (OEIS A023196).
   The function sieves the divisor–sum σ(k) up to a fixed limit
   and collects all k with σ(k) ≥ 2 k.  The array of results is
   built on first use and reused for subsequent calls. */

/* Includes --------------------------------------------------- */
#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, free */
#include <stdbool.h>  /* for bool */

/* Constants --------------------------------------------------- */
#define MAX_LIMIT 60000          /* maximum k to sieve, safe for 16‑bit seq_t */
#define INITIAL_CAPACITY 20000   /* initial size for the nondeficient list */

/* Static state ----------------------------------------------- */
static seq_t *nondef = NULL;
static size_t nondef_size = 0;      /* number of terms stored */
static size_t nondef_capacity = 0;  /* allocated capacity of nondef */
static bool initialized = false;

/* Helper: ensure data are ready -------------------------------- */
static void init_sequence(void)
{
    if (initialized)
        return;

    /* 1. Sieve σ(k) for k up to MAX_LIMIT using a 32‑bit accumulator. */
    uint32_t *sigma = (uint32_t*)calloc((size_t)MAX_LIMIT + 1, sizeof(uint32_t));
    if (!sigma)
        return; /* out of memory; in practice this should not happen */

    for (idx_t i = 1; i <= MAX_LIMIT; ++i) {
        for (idx_t j = i; j <= MAX_LIMIT; j += i)
            sigma[(size_t)j] += (uint32_t)i;
    }

    /* 2. Allocate storage for the nondeficient numbers. */
    nondef_capacity = INITIAL_CAPACITY;
    nondef = (seq_t*)malloc(nondef_capacity * sizeof(seq_t));
    if (!nondef) {
        free(sigma);
        return; /* out of memory */
    }
    nondef_size = 0;

    /* 3. Collect all k with σ(k) ≥ 2k. */
    for (idx_t k = 1; k <= MAX_LIMIT; ++k) {
        if (sigma[(size_t)k] >= 2 * (uint32_t)k) {
            if (nondef_size == nondef_capacity) {
                nondef_capacity *= 2;
                seq_t *tmp = (seq_t*)realloc(nondef, nondef_capacity * sizeof(seq_t));
                if (!tmp) { /* allocation failed, keep old array */
                    break;
                }
                nondef = tmp;
            }
            nondef[nondef_size++] = (seq_t)k;
        }
    }

    free(sigma);
    initialized = true;
}

/* Public API --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n < 1; return 0 as sentinel */
    init_sequence();
    if (n - 1 < nondef_size)
        return nondef[n - 1];
    return 0;              /* should not happen for n within the tested range */
}
