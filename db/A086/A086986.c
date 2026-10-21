/*
 * A086986  Numbers j such that sigma(j) can be represented as Sum_{i=0..k}
 *          sigma(m+i), with k>0.
 * https://oeis.org/A086986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 219, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A086986
   Numbers j such that σ(j) can be represented as a sum of at least two
   consecutive divisor–sums σ(m)+…+σ(m+k) (k≥1).
   The routine pre–computes σ up to a dynamically enlargeable limit,
   stores all sums of two or more consecutive σ values, and returns
   the j‑th term of the ordered list.  The algorithm runs in
   O(L log L + total window length) time for a current limit L and
   consumes only a few megabytes of memory.  */ 
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* global state for the sequence construction */
static idx_t *global_terms = NULL;   /* array of j values in increasing order */
static size_t  global_terms_size = 0;/* number of terms currently stored       */
static idx_t   global_limit = 0;     /* maximum integer processed so far      */

/* build the sequence up to 'limit', store terms in global_* variables   */
static void build_sequence(idx_t limit)
{
    /* 1. allocate and compute σ values                                       */
    uint64_t *sigma = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    if (!sigma) { exit(1); }                 /* out of memory – abort        */
    memset(sigma, 0, (size_t)(limit + 1) * sizeof(uint64_t));

    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            sigma[(size_t)j] += (uint64_t)i;
        }
    }

    /* 2. find maximum σ to size auxiliary arrays                            */
    uint64_t maxSigma = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (sigma[(size_t)i] > maxSigma) maxSigma = sigma[(size_t)i];
    }

    /* 3. allocate sumsExists array (uint8_t as boolean flag)                 */
    uint8_t *sumsExists = (uint8_t *)calloc((size_t)maxSigma + 1, sizeof(uint8_t));
    if (!sumsExists) { free(sigma); exit(1); }

    /* 4. prefix sums to compute consecutive sums quickly                    */
    uint64_t *prefix = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    if (!prefix) { free(sigma); free(sumsExists); exit(1); }
    prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        prefix[(size_t)i] = prefix[(size_t)(i - 1)] + sigma[(size_t)i];
    }

    /* 5. enumerate all sums of ≥2 consecutive σ values                       */
    for (idx_t start = 1; start <= limit; ++start) {
        /* start+1 is the first end that gives at least two terms */
        for (idx_t end = start + 1; end <= limit; ++end) {
            uint64_t sum = prefix[(size_t)end] - prefix[(size_t)(start - 1)];
            if (sum > maxSigma) break;          /* further sums will only grow   */
            sumsExists[(size_t)sum] = 1;
        }
    }

    /* 6. collect all j with σ(j) present in sumsExists                       */
    /*    Allocate an array large enough to hold up to 'limit' terms          */
    idx_t *new_terms = (idx_t *)malloc((size_t)limit * sizeof(idx_t));
    if (!new_terms) { free(sigma); free(prefix); free(sumsExists); exit(1); }
    size_t new_size = 0;
    for (idx_t j = 1; j <= limit; ++j) {
        if (sumsExists[(size_t)sigma[(size_t)j]]) {
            new_terms[new_size++] = j;
        }
    }

    /* 7. replace global state with the new data                              */
    free(global_terms);          /* free old terms array (if any)    */
    global_terms = new_terms;
    global_terms_size = new_size;
    global_limit = limit;

    /* 8. free temporary arrays                                               */
    free(sigma);
    free(prefix);
    free(sumsExists);
}

/* ------------------------------------------------------------------------- */
/*  Public entry point: returns the n-th term of the sequence                 */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static idx_t current_limit = 200000;   /* initial pre‑computation range   */

    if (!initialized) {
        /* build initial sequence */
        build_sequence(current_limit);

        /* ensure we have at least 10000 terms for rapid access              */
        while (global_terms_size < 10000) {
            current_limit *= 2;
            build_sequence(current_limit);
        }
        initialized = 1;
    }

    /* if n is beyond the currently available terms, enlarge the range     */
    while (global_terms_size < n) {
        current_limit *= 2;
        build_sequence(current_limit);
    }

    /* sequence is 1‑based */
    return (seq_t)global_terms[(size_t)(n - 1)];
}
