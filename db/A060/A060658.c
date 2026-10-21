/*
 * A060658  Even numbers n such that sigma(x) = n has no solution.
 * https://oeis.org/A060658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A060658: even numbers n such that σ(x)=n has no solution.
   The implementation builds the range of σ(x) values up to an
   increasing limit and collects all even integers not hit by σ.
   It keeps a static cache of the missing numbers so that arbitrary
   calls to a(idx_t n) are answered quickly. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/append_candidate.h"

static seq_t *missing = NULL;          /* array of missing even numbers */
static size_t missing_len = 0;         /* current number of stored values  */
static size_t missing_cap = 0;         /* capacity of missing              */

/* previously computed bound for σ calculation; all numbers ≤ prev_limit
   have already been processed for missing evens. */
static size_t prev_limit = 0;

/* Build missing even numbers up to a new absolute bound `limit`.  New
  > previous limit.  All values ≤ prev_limit are already stored. */
static void build_missing_up_to(size_t limit)
{
    /* Allocate buffers for σ and presence array. */
    seq_t *sigma = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    bool *present = (bool *)malloc((limit + 1) * sizeof(bool));
    assert(sigma != NULL && present != NULL);

    for (size_t i = 0; i <= limit; ++i) {
        sigma[i] = 0;
        present[i] = false;
    }

    /* Compute σ(x) for all x ≤ limit using the classic divisor-sum sieve. */
    for (size_t i = 1; i <= limit; ++i) {
        seq_t s = (seq_t)i;
        for (size_t j = i; j <= limit; j += i) {
            sigma[j] += s;
        }
    }

    /* Mark all values that appear as σ(x) for some x ≤ limit. */
    for (size_t x = 1; x <= limit; ++x) {
        seq_t val = sigma[x];
        if (val <= (seq_t)limit) {
            present[(size_t)val] = true;
        }
    }

    /* Scan the new range beyond prev_limit and collect missing evens. */
    size_t start = (prev_limit < 2) ? 2 : prev_limit + 2;
    for (size_t val = start; val <= limit; val += 2) {
        if (!present[val]) {
            append_candidate(&missing, &missing_len, &missing_cap, (seq_t)val);
        }
    }

    /* Clean up temporary arrays. */
    free(sigma);
    free(present);

    /* Update the previous limit. */
    prev_limit = limit;
}

/* External API required by the test harness. */
seq_t a(idx_t n)
{
    /* Ensure we have computed enough missing numbers for the requested
       index.  The first call will start with a moderate bound; we
       double the bound until the cache is large enough. */
    static size_t current_limit = 0;
    while (missing_len < (size_t)n) {
        if (current_limit == 0) {
            current_limit = 50000;              /* initial bound */
        } else {
            current_limit <<= 1;                /* double if needed */
        }
        build_missing_up_to(current_limit);
    }
    return missing[n - 1];
}
