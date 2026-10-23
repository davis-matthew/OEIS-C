/*
 * A162594  Differences of cubes: T(n,n) = n^3, T(n,k) = T(n,k+1) - T(n-1,k),
 *          0 <= k < n, triangle read by rows.
 * https://oeis.org/A162594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th term of the “Differences of cubes” triangle.
 *            The triangle T is defined by T(n,n)=n³ and
 *            T(n,k)=T(n,k+1)-T(n-1,k) for 0≤k<n. The sequence a(n) reads the
 *            rows of T left‑to‑right, starting with a(0)=0
 * Range:     u8 n <= 34, u16 n <= 901, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root of a 64‑bit unsigned integer (floor). */
static idx_t _floor_isqrt_u64(idx_t n) {
    idx_t r = 0;
    for (int i = 62; i >= 0; --i) {
        idx_t cand = r | ((idx_t)1ULL << i);
        if (cand <= n / cand) r = cand;
    }
    return r;
}

seq_t a(idx_t n) {
    /* Find the row r and column k corresponding to index n. */
    idx_t t = (idx_t)8 * n + 1;
    idx_t r = (_floor_isqrt_u64(t) - 1) >> 1;          /* r = floor((sqrt(8n+1)-1)/2) */
    idx_t start = r * (r + 1) / 2;                    /* first index in row r */
    idx_t k = n - start;                              /* column in this row */

    /* Allocate temporary arrays for two consecutive rows. */
    uint64_t *prev = (uint64_t *)malloc((size_t)(r + 1) * sizeof(uint64_t));
    uint64_t *cur  = (uint64_t *)malloc((size_t)(r + 1) * sizeof(uint64_t));
    if (!prev || !cur) {                    /* allocation failure → return 0 */
        free(prev); free(cur);
        return (seq_t)0;
    }

    for (idx_t row = 0; row <= r; ++row) {
        /* T(row,row) = row³ */
        uint64_t cube = (uint64_t)row * (uint64_t)row * (uint64_t)row;
        cur[row] = cube;

        /* Compute remaining entries of the row from right to left. */
        for (int j = (int)row - 1; j >= 0; --j) {
            cur[j] = cur[j + 1] - prev[j];
        }

        if (row == r) {                     /* desired entry found */
            seq_t result = (seq_t)cur[k];
            free(prev); free(cur);
            return result;
        }

        /* Prepare for next row: swap pointers. */
        uint64_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* This point is unreachable. */
    free(prev); free(cur);
    return (seq_t)0;
}
