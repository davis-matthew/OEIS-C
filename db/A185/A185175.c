/*
 * A185175  a(n) = A010815(7*n + 5).
 * https://oeis.org/A185175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A010815(7*n + 5) – Euler transform of the 7‑periodic
 *            sequence c = [-1, 0, 1, 1, 0, -1, -1], i.e. ∏_{k≥1} (1 -
 *            x^k)^{-c_{k-1}} = Σ a(n)x^n
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the coefficients up to the largest n
   requested.  All values are stored in a static array of type seq_t
   (which may be an arbitrary‑precision integer class).  The helper
   functions seq_alloc_array and seq_free_array are used for memory
   management to avoid issues with non‑POD types in the C++ build. */

#include <stdbool.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* 7‑periodic sequence c_k;  c[(k‑1)%7] gives the exponent factor. */
static const int period[7] = { -1, 0, 1, 1, 0, -1, -1 };

/* Cached coefficient array and its length. */
static seq_t *coeff = NULL;
static idx_t coeff_len = 0;               /* size of coeff array (elements 0..coeff_len-1) */
static bool   initialized = false;

/* Recompute all coefficients up to N (inclusive).  The array is
   reallocated using the helper functions, and the old data is freed. */
static void compute_full(idx_t N)
{
    seq_t *newcoeff = seq_alloc_array((size_t)(N + 1));
    /* Initialize all entries to zero */
    for (idx_t i = 0; i <= N; ++i)
        newcoeff[i] = 0;

    newcoeff[0] = 1;

    for (idx_t k = 1; k <= N; ++k) {
        int p = period[(k - 1) % 7];   /* -1, 0, 1, 1, 0, -1, -1 */
        if (p == 0)
            continue;                  /* exponent is 0 → no change */

        if (p == -1) {                /* multiply by (1 - x^k) */
            for (idx_t i = N; i >= k; --i)
                newcoeff[i] -= newcoeff[i - k];
        } else {                       /* p == 1 → divide by (1 - x^k) */
            for (idx_t i = k; i <= N; ++i)
                newcoeff[i] += newcoeff[i - k];
        }
    }

    /* Replace old array with the new one */
    if (coeff != NULL)
        seq_free_array(coeff);
    coeff = newcoeff;
    coeff_len = N + 1;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (!initialized) {             /* first call – build up to 10 000 */
        compute_full(10000);
        initialized = true;
    }

    if (n >= coeff_len) {           /* need more terms: grow array */
        compute_full(n);
    }

    return coeff[n];
}
