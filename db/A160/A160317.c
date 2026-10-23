/*
 * A160317  Numerator of Hermite(n, 19/31).
 * https://oeis.org/A160317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A160317: a(n) = 31^n * Hermite(n,19/31)
   Recurrence: a(0)=1, a(1)=38
                a(n+2) = 38*a(n+1) - 1922*(n+1)*a(n)   (n≥0)
*/

/* Include necessary headers */
#include <stddef.h>          /* size_t */
#include "../utils/grow_seq_array.h"   /* grow_seq_array */

/* Static cache for memoisation */
static seq_t *cache = NULL;          /* array of pre‑computed terms */
static size_t  computed = 0;         /* number of terms already computed (valid indices 0..computed-1) */

/* Ensure that cache is big enough and that terms up to n are computed */
static void ensure_cache(idx_t n)
{
    /* Convert index to size_t for array indexing */
    size_t N = (size_t)n;

    /* If nothing is allocated yet, grow to at least N+1 */
    size_t old_computed = computed;
    if (N + 1 > old_computed) {
        if (cache == NULL) {
            /* Allocate first array */
            grow_seq_array(&cache, &computed, N + 1);
            old_computed = 0;                /* nothing was computed before */
        } else {
            /* Expand existing array */
            grow_seq_array(&cache, &computed, N + 1);
        }
        /* Compute the newly added terms */
        for (size_t i = old_computed; i <= N; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else if (i == 1) {
                cache[1] = 38;
            } else {
                seq_t term = 38 * cache[i - 1];
                term -= ((seq_t)1922) * ((seq_t)(i - 1)) * cache[i - 2];
                cache[i] = term;
            }
        }
        computed = N + 1;
    }
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
