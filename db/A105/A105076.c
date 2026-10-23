/*
 * A105076  Numbers k such that 60*k^2 + 60*k + 1 is a square.
 * https://oeis.org/A105076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 8*a(n-2) - 8*a(n-3) - a(n-4) + a(n-5), a(1)=0,
 *            a(2)=1, a(3)=2, a(4)=11, a(5)=19
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* Off‑by‑one guard, not formally required */

    /* static storage for the sequence values */
    static seq_t *seq = NULL;            /* array a(1) .. a(max_cached) */
    static size_t cap = 0;               /* current allocation size */
    static idx_t stored = 0;             /* number of terms actually computed */

    /* Temporary initialization (first call) */
    if (cap == 0) {
        /* Allocate at least 5 entries for the known initial terms */
        ensure_capacity(5, &seq, &cap);

        seq[0] = (seq_t)0;   /* a(1) */
        seq[1] = (seq_t)1;   /* a(2) */
        seq[2] = (seq_t)2;   /* a(3) */
        seq[3] = (seq_t)11;  /* a(4) */
        seq[4] = (seq_t)19;  /* a(5) */
        stored = 5;
    }

    /* Extend the cache if the requested index is beyond the current range */
    if (n > stored) {
        /* Ensure the array is large enough */
        ensure_capacity((size_t)n, &seq, &cap);

        /* Compute successive terms using the 5‑term linear recurrence */
        for (idx_t i = stored + 1; i <= n; ++i) {
            /* Positive part: a(i‑1) + 8·a(i‑2) + a(i‑5) */
            seq_t pos = seq[i-2] + ((seq_t)8 * seq[i-3]) + seq[i-6];

            /* Negative part: 8·a(i‑3) + a(i‑4) */
            seq_t neg = ((seq_t)8 * seq[i-4]) + seq[i-5];

            /* Result is guaranteed non‑negative */
            seq[i-1] = pos - neg;
        }
        stored = n;
    }

    return seq[n-1];
}
