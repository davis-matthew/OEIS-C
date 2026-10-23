/*
 * A101981  Column 0 of triangle A101980, which is the matrix logarithm of
 *          A008459 (squared entries of Pascal's triangle).
 * https://oeis.org/A101981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 14, i128 n <= 22,
 *            big any n
 * Helpers:   binom, resize_array, seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A101981
    Formula: a(0) = 0,
             a(1) = 1,
             for n >= 2:
             a(n) = 1 - Σ_{j=1}^{n-1} binomial(n, j)*binomial(n-1, j-1)*a(j) */

/*  The function uses a static memoised table so that any order of calls
    is handled efficiently.  The recurrence is O(n²) for the first n
    values computed.  All operations are performed on seq_t which may be
    an arbitrary‑precision type, so the implementation works for all
    supported integer sizes. */

#include <stdint.h>
#include <stddef.h>
#include <string.h>   /* for memcpy */

/* The following helpers are provided by the build system. */
#include "../utils/binom.h"
#include "../utils/seq_malloc.h"
#include "../utils/resize_array.h"

/* Static storage for memoisation */
static seq_t *a_vals = NULL;          /* array of computed a(n) values */
static idx_t  a_size = 0;             /* number of entries currently stored (a_vals[0..a_size-1]) */
static idx_t  a_cap  = 0;             /* allocated capacity */

/* Ensure that values up to and including n are computed and stored. */
static void
ensure_values(idx_t n)
{
    if (n < a_size) return;   /* already computed */

    /* Increase capacity if necessary */
    if (n + 1 > a_cap) {
        idx_t new_cap = (a_cap == 0) ? 1 : a_cap;
        while (new_cap <= n) new_cap <<= 1;
        a_vals = resize_array(a_vals, a_size, new_cap);
        a_cap = new_cap;
    }

    /* Compute new values */
    for (idx_t k = a_size; k <= n; ++k) {
        if (k == 0) {
            a_vals[k] = 0;
            continue;
        }
        if (k == 1) {
            a_vals[k] = 1;
            continue;
        }

        seq_t sum = 0;
        seq_t C1 = (seq_t)k;          /* binom(k,1) */
        seq_t C2 = 1;                 /* binom(k-1,0) */

        for (idx_t j = 1; j < k; ++j) {
            seq_t prod = C1 * C2;
            sum += prod * a_vals[j];

            /* Update C1 and C2 for next j, but avoid divisions when no next term exists */
            if (j + 1 < k) {
                C1 = C1 * ((seq_t)k - j) / ((seq_t)(j + 1));
                C2 = C2 * ((seq_t)k - j) / ((seq_t)j);
            }
        }
        a_vals[k] = 1 - sum;
    }
    a_size = n + 1;
}

/* Public API function */
seq_t
a(idx_t n)
{
    ensure_values(n);
    return a_vals[n];
}
