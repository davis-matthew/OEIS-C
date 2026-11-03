/*
 * A222350  Number of (n+6)X1 arrays of occupancy after each element moves up
 *          to +-n places including 0.
 * https://oeis.org/A222350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of occupancy arrays of length n+6 obtainable
 *            * by letting each of the n+6 elements move at most n positions
 *            left or * right (including staying). The sequence has offset 1
 *            and is defined for all * n ≥ 1. The count can be written as * *
 *            a(n) = Σ_{s=0}^{n+5} Σ_{t=0}^{n+5} Lleft[s]...
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard C headers */
#include <stdlib.h>
#include <stdint.h>
/* Utility for binomial coefficients */
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* ------------------------------------------------------------------------ */
static void dfs_brute(idx_t pos, idx_t rem, seq_t pref, idx_t *arr,
                      idx_t n, idx_t L, seq_t *cnt)
{
    if (pos == L) {
        if (rem == 0) {
            /* Check suffix constraints.
             * For the last five positions, the cumulative number of element
             * placements must not exceed n + (length of suffix) .           */
            seq_t suff = 0;
            for (idx_t j = L; j-- > 0; ) {
                suff += arr[j];
                idx_t suffix_len = L - 1 - j;          /* 0 … 4   */
                if (suffix_len <= 4 && suff > (seq_t)(n + suffix_len + 1))
                    return;
            }
            *cnt += 1;
        }
        return;
    }

    /* Upper bound for the prefix sum (only the first five positions).   */
    seq_t max_pref = (pos <= 4) ? (seq_t)(n + pos + 1) : (seq_t)(-1);
    for (idx_t val = 0; val <= rem; ++val) {
        seq_t new_pref = pref + (seq_t)val;
        if (pos <= 4 && new_pref > max_pref)
            break;          /* larger val would only increase the sum.    */
        arr[pos] = (idx_t)val;
        dfs_brute(pos + 1, rem - val, new_pref, arr, n, L, cnt);
    }
}

/* ------------------------------------------------------------------------ */
/* Helper for n ≤ 4: brute‑force enumeration of all admissible occupancy   */
/* vectors.  Recursively assign values to each of the L = n+6 positions.    */

/* ------------------------------------------------------------------------ */
/* Main sequence generation routine                                       */
/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* --------  Small n (≤ 4) – brute force -------------------------------- */
    if (n <= 4) {
        idx_t L = n + 6;
        idx_t  arr[20];                     /* L ≤ 10 for n ≤ 4          */
        seq_t  counter = 0;
        dfs_brute(0, L, 0, arr, n, L, &counter);
        return counter;
    }

    /* --------  n > 4 – use analytic formula -------------------------------- */
    idx_t L   = n + 6;                     /* total length of array      */
    idx_t M   = n - 4;                     /* number of middle (unconstrained) cells */
    size_t maxSum = (size_t)(n + 5);        /* sums that may appear on each side */

    /* Allocate arrays for DP over the first five positions (prefix side). */
    size_t size = maxSum + 2;              /* allow index up to n+5         */
    seq_t *dp  = mem_new(seq_t, size);
    seq_t *next= mem_new(seq_t, size);
    if (!dp || !next) {                   /* In case of allocation failure */
        if (dp) mem_free(dp);
        if (next) mem_free(next);
        return 0;
    }

    dp[0] = 1;                             /* initial empty configuration  */

    /* DP over the first five cells, respecting their upper bounds.      */
    for (int j = 0; j < 5; ++j) {
        size_t U_curr = (size_t)(n + j);    /* current maximum achievable sum */
        size_t U_next = U_curr + 1;         /* after adding the next cell     */

        seq_t sum = 0;
        for (size_t s = 0; s <= U_next; ++s) {
            if (s <= U_curr)
                sum += dp[s];
            next[s] = sum;
        }
        /* Copy back to dp for the next iteration.                          */
        for (size_t s = 0; s <= U_next; ++s)
            dp[s] = next[s];
    }

    /* dp[s] now holds the number of ways to populate the first five cells
     * with a total of s elements, subject to the prefix constraints. */
    /* Pre‑compute binomial coefficients C(L - k + M - 1 , M - 1).          */
    size_t combSize = (size_t)L + 1;
    seq_t *comb = mem_new(seq_t, combSize);
    for (size_t k = 0; k <= L; ++k) {
        idx_t R = L - k;                   /* elements left for the middle etc. */
        if (R >= 0)
            comb[k] = binomial((idx_t)(R + M - 1), (idx_t)(M - 1));
        else
            comb[k] = 0;
    }

    /* Convolution of the left and right side counts, weighted by the
     * corresponding binomial term.                                            */
    seq_t result = 0;
    for (size_t s = 0; s <= maxSum; ++s) {
        seq_t leftWays = dp[s];
        if (leftWays == 0) continue;
        for (size_t t = 0; t <= maxSum; ++t) {
            seq_t rightWays = dp[t];
            if (rightWays == 0) continue;
            size_t k = s + t;
            if (k > L) continue;           /* remainder would be negative   */
            seq_t term = leftWays * rightWays;
            term = term * comb[k];
            result += term;
        }
    }

    /* Free allocated memory.                                                      */
    mem_free(dp);
    mem_free(next);
    mem_free(comb);

    return result;
}
