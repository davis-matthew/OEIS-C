/*
 * A093056  Triangle T(j,k) read by rows, where T(j,k) = length of the longest
 *          cycle in the in-situ transposition of a rectangular j X k matrix.
 * https://oeis.org/A093056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a
   Computes the n-th term of OEIS A093056 (Triangle T(j,k) read by rows).
   The triangle is built for all rectangles with 2 ≤ k ≤ j.
   For a given n the pair (j,k) is recovered from the triangular indexing,
   and the longest cycle length of the in‑situ transposition of the
   j×k matrix is found by a simple cycle detection algorithm.
   All arithmetic uses unsigned 64‑bit integers, with 128‑bit products where
   necessary to avoid overflow.  The routine works for any allowed value
   of idx_t and returns a seq_t that can hold the result. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: product in 128 bits to avoid overflow. */
static inline __uint128_t mul128(idx_t a, idx_t b) {
    return (__uint128_t)a * b;
}

/* Helper: find smallest j ≥ 2 such that j*(j-1)/2 ≥ n. */
static idx_t find_row(idx_t n) {
    /* Double until high satisfies condition. */
    idx_t low = 2, high = 4;
    while (1) {
        __uint128_t tri = mul128(high, high - 1) / 2;
        if (tri >= n) break;
        high <<= 1; /* multiply by 2 */
    }
    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t tri = mul128(mid, mid - 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    /* Find row j and column k corresponding to index n. */
    idx_t j = find_row(n);                /* j ≥ 2 */
    idx_t prev = mul128(j - 2, j - 1) / 2; /* sum of earlier rows */
    idx_t offset = n - prev - 1;           /* 0‑based offset within row */
    idx_t k = 2 + offset;                 /* column index */

    /* Size of permutation: M = j * k. */
    idx_t M = mul128(j, k);

    /* Allocate visited array. */
    uint8_t *visited = (uint8_t *)calloc((size_t)M, 1);
    if (!visited) return 0; /* allocation failure */

    seq_t max_len = 0;

    for (idx_t start = 0; start < M; ++start) {
        if (visited[start]) continue;
        /* Traverse cycle starting at 'start'. */
        idx_t cur = start;
        seq_t len = 0;
        do {
            visited[cur] = 1;
            /* mapping: P(p) = (p % k) * j + (p / k) */
            cur = (cur % k) * j + (cur / k);
            ++len;
        } while (cur != start);

        if (len > max_len) max_len = len;
    }

    free(visited);
    return max_len;
}
