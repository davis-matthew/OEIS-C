/*
 * A112275  Smallest number greater than n having at least as many divisors as
 *          n.
 * https://oeis.org/A112275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest number > n having at least as many divisors as n
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;
static seq_t *div_counts = NULL;
static idx_t div_limit = 0;
static const idx_t INITIAL_LIMIT = 200000;

/* Ensure that divisor counts are computed up to at least 'needed'. */
static void
ensure_div_limit(idx_t needed)
{
    if (needed <= div_limit) return;

    idx_t new_limit = needed + 100;
    if (div_limit == 0) new_limit = INITIAL_LIMIT;
    if (new_limit < div_limit * 2) new_limit = div_limit * 2;

    /* Reallocate array. */
    seq_t *new_arr = (seq_t *)realloc(div_counts, (new_limit + 1) * sizeof(seq_t));
    if (!new_arr) return; /* In case of allocation failure, leave unchanged. */
    div_counts = new_arr;
    div_limit = new_limit;

    /* Recompute divisor counts from scratch up to new_limit. */
    for (idx_t i = 0; i <= div_limit; ++i) div_counts[i] = 0;
    div_counts[1] = 1;
    for (idx_t i = 2; i <= div_limit; ++i) {
        for (idx_t j = i; j <= div_limit; j += i) {
            div_counts[j] = div_counts[j] + 1;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n < 1 */
    if (!initialized) {
        ensure_div_limit(INITIAL_LIMIT);
        initialized = true;
    }
    if (n > div_limit) ensure_div_limit(n);
    seq_t dn = div_counts[n];
    idx_t candidate = n + 1;
    while (true) {
        if (candidate > div_limit) ensure_div_limit(candidate);
        if (div_counts[candidate] >= dn) break;
        ++candidate;
    }
    return (seq_t)candidate;
}
