/*
 * A055431  Number of points in Z^n of norm <= 7.
 * https://oeis.org/A055431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integer vectors in ℤⁿ with Euclidean norm ≤ 7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 27, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming: keep counts of the number of vectors
   with a given sum of squares and extend dimension by dimension. */

#include <stdlib.h>

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Static state for memoization across calls. */
    static int init = 0;
    static int computed_until = -1;  /* highest n computed */
    static seq_t *a_vals = NULL;     /* array of a(n) values */
    static size_t a_cap = 0;         /* capacity of a_vals */

    /* DP arrays for counts of vectors with given sum of squares (0..49). */
    static seq_t prev[50];
    static seq_t curr[50];

    /* One–time initialization. */
    if (!init) {
        init = 1;
        for (int i = 0; i < 50; ++i) {
            prev[i] = 0;
        }
        prev[0] = 1;                /* a(0) = 1 */

        a_cap = 16;
        a_vals = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_vals[0] = 1;              /* a(0) */
        computed_until = 0;
    }

    /* Extend precomputation up to requested n. */
    while (computed_until < n) {
        /* Reset current DP array. */
        for (int i = 0; i < 50; ++i) {
            curr[i] = 0;
        }

        /* Transition: add a new coordinate x ∈ [-7,7]. */
        for (int s = 0; s < 50; ++s) {
            if (prev[s] == 0) continue;
            for (int x = -7; x <= 7; ++x) {
                int sq = x * x;          /* 0..49 */
                int ns = s + sq;
                if (ns < 50) {
                    curr[ns] += prev[s];
                }
            }
        }

        /* Sum all counts to get a(computed_until+1). */
        seq_t next_val = 0;
        for (int i = 0; i < 50; ++i) {
            next_val += curr[i];
        }

        /* Store the result, enlarging the array if needed. */
        if ((size_t)(computed_until + 1) >= a_cap) {
            a_cap = a_cap ? a_cap * 2 : 16;
            a_vals = (seq_t *)realloc(a_vals, a_cap * sizeof(seq_t));
        }
        a_vals[computed_until + 1] = next_val;

        /* Prepare for next iteration. */
        for (int i = 0; i < 50; ++i) {
            prev[i] = curr[i];
        }
        ++computed_until;
    }

    return a_vals[n];
}
