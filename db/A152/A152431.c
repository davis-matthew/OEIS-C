/*
 * A152431  Eigentriangle, row sums = A000110, the Bell numbers.
 * https://oeis.org/A152431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nonn, tabl
 * Formula:   a(n) = A[r - c + 1] * B[c-1] where B[0] = 1 is the 0‑th Bell
 *            number, B[m] = ∑_{k=0}^{m-1} binom(m-1,k) * B[k] (m ≥ 1) A[1] =
 *            B[1] A[n] = B[n] - ∑_{k=1}^{n-1} A[k] * B[n-k] (n ≥ 2) and r, c
 *            are row/column indices obtained from the flat index n with
 *            offset 1. This implements the OEIS triangle A152431
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary utils */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binom.h"

/* global state for memoization */
static idx_t max_r = 0;          /* highest row already computed */
static seq_t *B = NULL;          /* Bell numbers: B[0]..B[max_r]          */
static seq_t *A = NULL;          /* A074664 sequence: A[1]..A[max_r]      */
static idx_t B_size = 0;         /* allocated size of B array             */
static idx_t A_size = 0;         /* allocated size of A array             */

/* Ensure Bell numbers B[0..r] and A[1..r] are computed */
static void ensure_up_to(idx_t r)
{
    if (r <= max_r)
        return;

    /* Allocate/extend Bell array (needs indices 0..r) */
    if (B_size < r + 1) {
        B = (seq_t *)realloc(B, (r + 1) * sizeof(seq_t));
        B_size = r + 1;
    }

    /* Allocate/extend A array (needs indices 1..r) */
    if (A_size < r + 1) {
        A = (seq_t *)realloc(A, (r + 1) * sizeof(seq_t));
        A_size = r + 1;
    }

    /* Ensure B[0] is initialized */
    if (max_r == 0 && r >= 0)
        B[0] = (seq_t)1;

    /* Compute Bell numbers up to B[r] */
    for (idx_t n = max_r + 1; n <= r; ++n) {
        seq_t sum = 0;
        for (idx_t k = 0; k < n; ++k) {
            seq_t bin = binom((idx_t)n - 1, k);
            sum += bin * B[k];
        }
        B[n] = sum;
    }

    /* Compute A074664 values up to A[r] */
    if (r >= 1) {
        if (max_r < 1)        /* A[1] depends on B[1] */
            A[1] = B[1];

        for (idx_t n = max_r + 1; n <= r; ++n) {
            seq_t sum = 0;
            for (idx_t k = 1; k < n; ++k) {
                sum += A[k] * B[n - k];
            }
            A[n] = B[n] - sum;
        }
    }

    max_r = r;
}

/* ------------------------------------------------------------------ */
/* External function required by the build system.                     */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1, but guard */

    /* Find row r such that (r-1)*r/2 < n ≤ r*(r+1)/2. */
    idx_t r = 1;
    while (((r * (r + 1)) / 2) < n)
        ++r;

    idx_t base = (r - 1) * r / 2;
    idx_t c = n - base;          /* column within the row   */
    idx_t m = r - c + 1;         /* index for A            */

    ensure_up_to(r);

    return A[m] * B[c - 1];
}
