/*
 * A077768  Number of times that the sum of two squares is an integer between
 *          n^2 and (n+1)^2; multiple representations are counted multiply.
 * https://oeis.org/A077768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (x,y) ∈ ℕ² : 0 ≤ y ≤ x and n² < x² + y² < (n+1)² }
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   u64_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation pre–computes all relevant sums up to (max_n+1)²
   with an integer‑square‑root routine written without floating point,
   stores the counts in a static array, and answers each query in O(1). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/u64_isqrt.h"

/* ------------------------------------------------------------------ */
/* Integer square‑root of a 64‑bit unsigned value, floor(√x).       */

/* ------------------------------------------------------------------ */
/* Static state for memoisation.  counts[1..n] stores a(i).             */
/* ------------------------------------------------------------------ */
static seq_t *counts = NULL;
static idx_t  max_n  = 0;

/* Recompute the table for all n in [1 .. limit] (inclusive).           */
static void recompute_upto(idx_t limit)
{
    if (limit == 0) return;

    /* Allocate a new array.  Index 0 is unused. */
    seq_t *new_counts = (seq_t *)calloc(limit + 1, sizeof(seq_t));
    if (!new_counts) abort();          /* out of memory */

    /* Main double loop over unordered pairs (a,b) with a ≥ b.           */
    for (idx_t a = 0; a <= limit + 1; ++a) {
        uint64_t a_sq = (uint64_t)a * (uint64_t)a;   /* a² */

        for (idx_t b = 0; b <= a; ++b) {
            uint64_t sum = a_sq + (uint64_t)b * (uint64_t)b;   /* a² + b² */

            if (sum <= 1) continue;          /* never lies between squares */

            /* Skip perfect squares; they are not inside an open interval. */
            uint64_t sroot = u64_isqrt(sum);
            if (sroot * sroot == sum) continue;

            /* Determine n: n² < sum < (n+1)²   →   n = floor(√(sum‑1)). */
            uint64_t n_root = u64_isqrt(sum - 1);
            if (n_root == 0) continue;       /* should not happen for sum>1 */
            if (n_root <= limit) {
                /* Increment the counter; wrap around is acceptable when
                   seq_t’s width is exceeded, which only occurs if the
                   sequence value does not fit in seq_t.                  */
                new_counts[(idx_t)n_root] += 1;
            }
        }
    }

    /* Replace the old table. */
    free(counts);
    counts = new_counts;
    max_n  = limit;
}

/* ------------------------------------------------------------------ */
/* Public function required by the framework.                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0, defensive */
    if (n > max_n) recompute_upto(n);    /* compute up to requested n */
    return counts[n];
}
