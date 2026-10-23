/*
 * A137679  Triangle read by rows, A000012 * A008284.
 * https://oeis.org/A137679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows, A000012 * A008284.
   a(n) is the element at index n in the 1‑based linear read
   of the product triangle.
   The product of the lower–triangular matrices A000012 (all ones)
   and A008284 (partitions of n into exactly k parts) yields
   entries
        T[r][c] = Σ_{i=c}^{r} p(i,c)  (row r, column c)
   where p(i,c) is the number of partitions of i into exactly c parts.
   a(n) is T[r][c] with r,c determined by the position of n in the
   triangular ordering.  The algorithm computes the required p(i,k)
   values on demand, storing them in a static table.  It runs in
   O(r) time per call, where r ≈ √(2n). */

#include <stdint.h>
#include <stdlib.h>

/* Utility: floor integer square root of a 64‑bit unsigned value. */
#include "../utils/isqrt.h"

/* Static table of partition numbers p[n][k] for n,k ≤ current maximum. */
static seq_t **P = NULL;
static idx_t Pmax = 0;

/* Ensure that P contains values up to at least N.  The table
   is stored as a contiguous array of pointers to rows; each row i
   is a malloc'ed array of length i+1 (indices 0..i). */
static void ensureP(idx_t N)
{
    if (N <= Pmax) return;

    /* Expand the array of row pointers to size N+1. */
    seq_t **newP = (seq_t**)realloc(P, sizeof(seq_t*) * (N + 1));
    if (!newP) return;          /* allocation failure - ignore in this context */
    P = newP;

    /* Allocate row 0 if it does not exist yet. */
    if (Pmax == 0) {
        P[0] = (seq_t*)malloc(sizeof(seq_t));
        if (!P[0]) return;
        P[0][0] = (seq_t)1;     /* p(0,0)=1 */
    }

    /* Allocate new rows and compute their values. */
    for (idx_t i = Pmax + 1; i <= N; ++i) {
        P[i] = (seq_t*)malloc(sizeof(seq_t) * (i + 1));
        if (!P[i]) return;
    }

    for (idx_t m = Pmax + 1; m <= N; ++m) {
        P[m][0] = (seq_t)0;                  /* p(m,0)=0 for m>0 */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t part1 = P[m - 1][k - 1];
            seq_t part2 = (k <= m - k) ? P[m - k][k] : (seq_t)0;
            P[m][k] = part1 + part2;
        }
    }

    Pmax = N;
}

/* Determine the row number r (1‑based) for a given index n. */
static idx_t triangle_row(idx_t n)
{
    /* Use integer arithmetic: find the smallest r such that r*(r+1)/2 ≥ n. */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    return high;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of bounds, but undefined behaviour ok */

    /* Find row r and column c for the linear index n. */
    idx_t r = triangle_row(n);
    idx_t prev_total = (unsigned __int128)(r - 1) * r / 2;   /* Σ_{i=1}^{r-1} i */
    idx_t c = n - prev_total;          /* 1‑based column within row r */

    /* Ensure partition table up to row r. */
    ensureP(r);

    /* Compute the sum of partitions from i = c to r. */
    seq_t sum = 0;
    for (idx_t i = c; i <= r; ++i)
        sum += P[i][c];

    return sum;
}
