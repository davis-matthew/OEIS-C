/*
 * A026422  a(n) = least positive integer > a(n-1) and not a(i)*a(j) for 1 <=
 *          i <= j < n.
 * https://oeis.org/A026422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A026422: a(n) = least integer > a(n-1) that is not equal to
   a(i)*a(j) for 1 ≤ i ≤ j < n.  The sequence is generated on demand
   and memoised so that calls can be in any order. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/* Internal storage for computed terms (0‑based index). */
static seq_t *seq      = NULL;   /* array of terms */
static size_t seq_len  = 0;      /* number of terms stored (<= capacity) */
static size_t seq_cap  = 0;      /* current allocation capacity */

/* Binary search for value v in the sorted array seq[0..upto-1];
   returns true if found. */
static bool seq_contains(seq_t v, size_t upto)
{
    size_t lo = 0, hi = upto;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (seq[mid] < v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < upto) && (seq[lo] == v);
}

/* Tests whether x can be written as a product a(i)*a(j) with 1 ≤ i ≤ j < n
   (i.e. using only indices less than the current length seq_len). */
static bool is_product(seq_t x)
{
    seq_t sq = floor_sqrt(x);          /* largest integer ≤ sqrt(x) */
    for (size_t i = 0; i < seq_len && seq[i] <= sq; ++i) {
        if (x % seq[i] == 0) {
            seq_t d = x / seq[i];
            if (seq_contains(d, seq_len))
                return true;            /* found a valid factorisation */
        }
    }
    return false;
}

/* Generates one new term and appends it to seq[]. */
static void compute_next(void)
{
    /* Initialise the first term if required. */
    if (seq_len == 0) {
        seq_cap = 4;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq_len = 1;
        return;
    }

    seq_t candidate = seq[seq_len - 1] + (seq_t)1;  /* start after previous */
    while (is_product(candidate))
        ++candidate;                               /* skip forbidden numbers */

    /* Grow the array if necessary. */
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 4;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = candidate;
}

/* Public function: a(n)  (1‑based index).  Undefined behaviour for
   n < 1; such a call simply returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* out of range – not used by tests */

    /* Ensure we have computed terms up to index n. */
    while (seq_len < (size_t)n)
        compute_next();

    return seq[(size_t)n - 1];
}
