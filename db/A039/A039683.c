/*
 * A039683  Signed double Pochhammer triangle: expansion of
 *          x(x-2)(x-4)..(x-2n+2).
 * https://oeis.org/A039683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039683: signed double Pochhammer triangle read in row order
 * The n-th term is T(r, m) where r is the row containing index n and
 *   m = n – r(r‑1)/2.
 * T satisfies
 *   T(1,1) = 1,
 *   T(n,m) = T(n‑1,m‑1) – 2·(n‑1)·T(n‑1,m)   for n ≥ m ≥ 1,
 *   T(n,0) = 0,   T(n,m) = 0  for m > n.
 *
 * The implementation computes the required row using dynamic
 * programming and returns the element at position m.
 *
 * All operations are performed in the seq_t type; indices are in
 * idx_t (unsigned 64‑bit).  No external helper is required.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square‑root: floor(sqrt(x)) for 0 ≤ x < 2^64.
   Uses a bit‑shifting method that works with 64‑bit arithmetic only. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t res = 0;
    idx_t bit = 1ull << 62;        /* the highest power of four <= 2^64 */
    while (bit > 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Return the a(n) term of A039683 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* Find the row r with  r(r‑1)/2 < n ≤ r(r+1)/2  */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;
    if (r * (r + 1) / 2 < n) r++;   /* adjust if not already covering n */

    /* Column index within the found row (1‑based) */
    idx_t m = n - r * (r - 1) / 2;   /* 1 ≤ m ≤ r */

    if (r == 1)                     /* trivial first row */
        return (seq_t)1;

    /* Allocate two rows of length up to r (index 0 unused). */
    seq_t *prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!prev || !curr) {            /* allocation failure */
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Initialize first row: T(1,1)=1, others 0. */
    for (idx_t i = 0; i <= r; ++i) prev[i] = 0;
    prev[1] = (seq_t)1;

    /* Build rows 2 … r */
    for (idx_t row = 2; row <= r; ++row) {
        /* First element of the current row: T(row,1) */
        curr[1] = - ((seq_t)2 * (seq_t)(row - 1) * prev[1]);

        /* Interior elements */
        for (idx_t col = 2; col <= row - 1; ++col) {
            curr[col] = prev[col - 1]
                       - ((seq_t)2 * (seq_t)(row - 1) * prev[col]);
        }

        /* Last element of the current row: T(row,row) */
        curr[row] = prev[row - 1];

        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[m];           /* T(r,m) is in prev[m] */
    free(prev);
    free(curr);
    return result;
}
