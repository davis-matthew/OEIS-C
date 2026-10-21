/*
 * A056276  Number of primitive (aperiodic) word structures of length n using
 *          a 5-ary alphabet.
 * https://oeis.org/A056276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * A056272(n/d) with
 *            A056272(n)=∑_{k=1}^{min(5,n)} S(n,k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes A056272(n) via the recurrence
   S(n,k)=k*S(n-1,k)+S(n-1,k-1) for k≤5, then evaluates a(n)
   using the relation A(n)=∑_{d|n} a(d).  No subtraction of
   negative numbers is required, which keeps the implementation
   correct for all unsigned seq_t types and arbitrary‑precision
   integer classes.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

static seq_t *A_arr = NULL;   /* A056272 values, A_arr[0] unused   */
static seq_t *a_arr = NULL;   /* a(n) values, a_arr[0] unused     */
static size_t cur_max = 0;    /* largest n for which values are precomputed */

/* Ensure that arrays are allocated up to at least n and compute
   all values up to n.  */
static void recompute(idx_t n)
{
    if ((size_t)n <= cur_max) return;

    /* Resize the arrays */
    A_arr = (seq_t *)realloc(A_arr, (n + 1) * sizeof(seq_t));
    a_arr = (seq_t *)realloc(a_arr, (n + 1) * sizeof(seq_t));

    /* 1. Compute A056272(k) for k = 1 .. n using Stirling numbers
       of the second kind S(k, i) for i = 0..5.  */
    seq_t curS[6] = {0}, newS[6] = {0};
    curS[0] = 1; /* S(0,0)=1,   S(0,i>0)=0 already zero  */
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute S(k, i) from S(k-1, i) */
        newS[0] = 0;
        for (int i = 1; i <= 5; ++i) {
            newS[i] = mul_small(curS[i], (unsigned int)i); /* i * S(k-1,i) */
            newS[i] = newS[i] + curS[i - 1];              /* + S(k-1,i-1) */
        }

        /* Sum over i=1..min(5,k) to get A(k) */
        seq_t total = 0;
        int up = (k < 5) ? (int)k : 5;
        for (int i = 1; i <= up; ++i)
            total = total + newS[i];
        A_arr[k] = total;

        /* Prepare for next iteration */
        for (int i = 0; i <= 5; ++i)
            curS[i] = newS[i];
    }

    /* 2. Compute a(n) using the relation A(n) = Σ_{d|n} a(d).  */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t sumProper = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                idx_t d1 = d;
                idx_t d2 = k / d;
                if (d1 < k) sumProper = sumProper + a_arr[d1];
                if (d2 != d1 && d2 < k) sumProper = sumProper + a_arr[d2];
            }
        }
        a_arr[k] = A_arr[k] - sumProper; /* A(k) ≥ sumProper, so no underflow */
    }

    cur_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */
    if ((size_t)n > cur_max) recompute(n);
    return a_arr[n];
}
