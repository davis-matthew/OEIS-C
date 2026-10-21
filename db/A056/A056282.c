/*
 * A056282  Number of primitive (aperiodic) word structures of length n which
 *          contain exactly six different symbols.
 * https://oeis.org/A056282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * A000770(n/d), where A000770(n) = S(n,6)
 *            (Stirling number of the second kind). Equivalent recurrence:
 *            a(n) = S(n,6) - Σ_{d|n, d<n} a(d). This implementation computes
 *            S(n,6) incrementally and memoises a(n) for all n. See OEIS
 *            A056282
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* We assume that seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n) {
    /* Static storage for Stirling numbers S(n,6) and memoised a(n). */
    static seq_t *S = NULL;          /* S[i] == S(i,6) */
    static idx_t S_max = 0;          /* largest index already computed for S */
    static seq_t prev_row[7];        /* previous row of Stirling recurrence */
    static seq_t next_row[7];        /* current row of Stirling recurrence */

    static seq_t *A = NULL;          /* memoised a(i) values, index 0 unused */
    static bool *A_done = NULL;      /* flag whether a(i) has been computed */
    static idx_t A_max = 0;          /* largest index already allocated for A */

    /* ------------------------------------------------------------------
       Ensure that we have S[0..n] available.  If necessary, grow the table
       and continue computing Stirling numbers from the last computed index
       using the recurrence S(n,j) = j*S(n-1,j) + S(n-1,j-1). */
    if (n > S_max) {
        idx_t new_max = n;
        size_t new_size = new_max + 1;

        seq_t *newS = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!newS) abort();

        /* Copy existing entries that are already computed. */
        if (S) {
            memcpy(newS, S, (S_max + 1) * sizeof(seq_t));
            free(S);
        }
        /* Initialise the new entries to zero. */
        for (idx_t i = S_max + 1; i <= new_max; ++i)
            newS[i] = (seq_t)0;

        /* Initialise prev_row for n = 0 if we are starting new. */
        if (S_max == 0) {
            prev_row[0] = 1;
            for (int j = 1; j <= 6; ++j)
                prev_row[j] = (seq_t)0;
        }

        /* Compute Stirling numbers for rows S_max+1 .. new_max. */
        for (idx_t i = S_max + 1; i <= new_max; ++i) {
            next_row[0] = (seq_t)0;
            for (int j = 1; j <= 6; ++j) {
                next_row[j] = (seq_t)j * prev_row[j] + prev_row[j - 1];
            }
            newS[i] = next_row[6];
            for (int j = 0; j <= 6; ++j)
                prev_row[j] = next_row[j];
        }

        S = newS;
        S_max = new_max;
    }

    /* ------------------------------------------------------------------
       Ensure that we have memoisation storage for a[0..n].  Grow if needed. */
    if (n > A_max) {
        idx_t new_max = n;
        size_t new_size = new_max + 1;

        seq_t *newA   = (seq_t *)malloc(new_size * sizeof(seq_t));
        bool   *newD  = (bool  *)malloc(new_size * sizeof(bool ));
        if (!newA || !newD) abort();

        if (A) {
            memcpy(newA,  A, (A_max + 1) * sizeof(seq_t));
            memcpy(newD, A_done, (A_max + 1) * sizeof(bool));
            free(A);
            free(A_done);
        }
        for (idx_t i = A_max + 1; i <= new_max; ++i) {
            newA[i]  = (seq_t)0;
            newD[i]  = false;
        }

        A      = newA;
        A_done = newD;
        A_max  = new_max;
    }

    /* ------------------------------------------------------------------
       Base cases: for n <= 5 the answer is 0.  These values are not
       cached because they are always zero. */
    if (n <= 5)
        return (seq_t)0;

    /* Return memoised result if already computed. */
    if (A_done[n])
        return A[n];

    /* Compute a(n) using the recurrence:
          a(n) = S(n,6) - Σ_{d|n, d<n} a(d)  */
    seq_t total = S[n];
    seq_t sum   = (seq_t)0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            if (d1 < n) sum += a(d1);
            if (d2 < n && d2 != d1)
                sum += a(d2);
        }
    }

    seq_t val = total - sum;
    A[n]      = val;
    A_done[n] = true;
    return val;
}
