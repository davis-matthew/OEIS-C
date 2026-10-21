/*
 * A078808  Triangular array T given by T(n,k) = number of 01-words of length
 *          n having exactly k 1's, all runlengths odd and first letter 1.
 * https://oeis.org/A078808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements OEIS A078808.
 * a(n) is the flattened triangle T(n,k): T(n,k) = #binary words of length n,
 * starting with 1, having exactly k ones, all run lengths odd.
 * The mapping from the flattened index n to (row r, column c) is:
 *   r = floor((sqrt(8*n+1)-1)/2),  c = n - r*(r+1)/2
 * The triangular values are computed by dynamic programming with two states:
 *   A[n][k] – words starting with 1, all runs odd, exactly k ones
 *   B[n][k] – words starting with 0, all runs odd, exactly k ones
 * Recurrence:
 *   B[n][k] = Σ_{odd l ≤ n} A[n-l][k]
 *   A[n][k] = Σ_{odd l ≤ n, k≥l} B[n-l][k-l]
 * Base: A[0][0] = B[0][0] = 1 (used to terminate the recursion).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t **A = NULL;                /* rows of A */
static seq_t **B = NULL;                /* rows of B */
static idx_t   dp_max = 0;              /* largest n for which A[n],B[n] are built */

/* Integer square‑root for idx_t values (unsigned 64‑bit). */
static idx_t floor_sqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid > 0 && mid > x / mid) hi = mid - 1;
        else { ans = mid; lo = mid + 1; }
    }
    return ans;
}

/* Ensure DP tables are computed up to 'needed'. */
static void ensure_dp(idx_t needed)
{
    if (needed <= dp_max) return;

    /* Allocate (or enlarge) the pointer arrays. */
    size_t new_size = (size_t)needed + 1;
    if (A == NULL) {
        A = (seq_t**)calloc(new_size, sizeof(seq_t*));
        B = (seq_t**)calloc(new_size, sizeof(seq_t*));
    } else {
        A = (seq_t**)realloc(A, new_size * sizeof(seq_t*));
        B = (seq_t**)realloc(B, new_size * sizeof(seq_t*));
    }

    /* Ensure row 0 exists – it is needed for all other rows. */
    if (A[0] == NULL) {
        A[0] = (seq_t*)calloc(1, sizeof(seq_t));
        B[0] = (seq_t*)calloc(1, sizeof(seq_t));
        A[0][0] = 1;   /* base for recursion */
        B[0][0] = 1;
    }

    for (idx_t n = dp_max + 1; n <= needed; ++n) {
        A[(size_t)n] = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
        B[(size_t)n] = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));

        /* Compute B[n][k] */
        for (idx_t k = 0; k <= n; ++k) {
            seq_t sum = 0;
            for (idx_t l = 1; l <= n; l += 2) {
                if (k <= n - l) sum += A[(size_t)n - l][(size_t)k];
            }
            B[(size_t)n][(size_t)k] = sum;
        }

        /* Compute A[n][k] */
        for (idx_t k = 0; k <= n; ++k) {
            seq_t sum = 0;
            for (idx_t l = 1; l <= n; l += 2) {
                if (k >= l && k - l <= n - l)
                    sum += B[(size_t)n - l][(size_t)k - l];
            }
            A[(size_t)n][(size_t)k] = sum;
        }
    }

    dp_max = needed;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* definition in OEIS */

    /* Find the triangular coordinates. */
    idx_t t = floor_sqrt_idx(8 * n + 1);
    idx_t r = (t - 1) / 2;
    idx_t row_start = r * (r + 1) / 2;
    idx_t c = n - row_start;

    if (r == 0) return 0;                /* T(0,0) is defined as 0 */

    ensure_dp(r);
    return A[(size_t)r][(size_t)c];
}
