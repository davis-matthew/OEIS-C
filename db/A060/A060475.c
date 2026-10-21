/*
 * A060475  Triangular array formed from successive differences of factorial
 *          numbers, then with factorials removed.
 * https://oeis.org/A060475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 51, u32 n <= 115, u64 n <= 250,
 *            u128 n <= 661, big any n
 * Helpers:   cum_before
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A060475 – Triangular array formed from successive differences
    of factorial numbers, then with factorials removed.
    Recurrence for the array T(n,k) (0 ≤ k ≤ n):
        T(0,0) = 1
        T(n,0) = 1
        T(n,k) = (n-k+1)*T(n,k-1) - T(n-1,k-1)   (1 ≤ k ≤ n)
    The flat sequence a(n) is obtained by reading the rows of T
    row‑major:  a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
    This implementation memoises the triangular array up to the
    required row; for n≈10 000 the largest row needed is 141,
    so memory usage and time are very small.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cum_before.h"

/* global storage for the triangle:  T[row] points to an array
   of (row+1) seq_t values */
static seq_t **T = NULL;
static idx_t rows_allocated = 0;

/* ensure that rows 0 … r are allocated and filled in T */
static void ensure_rows(idx_t r) {
    if (r < rows_allocated) return;

    /* grow the row pointer array */
    seq_t **new_T = (seq_t**)realloc(T, (r + 1) * sizeof(seq_t *));
    if (!new_T) abort();
    T = new_T;

    /* allocate and fill the new rows */
    for (idx_t i = rows_allocated; i <= r; ++i) {
        seq_t *row = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        if (!row) abort();
        T[i] = row;

        /* base value */
        row[0] = (seq_t)1;

        /* recurrence for k >= 1 */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t factor = (seq_t)(i - k + 1);      /* (n-k+1) */
            seq_t term   = factor * row[k - 1];     /* (n-k+1)*T(n,k-1) */
            seq_t sub    = T[i - 1][k - 1];         /* T(n-1,k-1) */
            row[k] = term - sub;                   /* recurrence */
        }
    }
    rows_allocated = r + 1;
}

/* main sequence function */
seq_t a(idx_t n) {
    /* find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n;
    idx_t r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t cum = (idx_t)(((__uint128_t)mid * (mid + 1)) / 2);
        if (cum <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* column index within that row */
    idx_t k = n - (idx_t)(((__uint128_t)r * (r + 1)) / 2);

    /* make sure the required row is present */
    ensure_rows(r);

    /* return the requested entry */
    return T[r][k];
}
