/*
 * A075498  Stirling2 triangle with scaled diagonals (powers of 3).
 * https://oeis.org/A075498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 80, u64 n <= 195,
 *            u128 n <= 569, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075498:  Stirling2 triangle with scaled diagonals (powers of 3)
   Flattened in row‑major order.
   a(n) = a(row, col) where
   row  = smallest r such that r*(r+1)/2 ≥ n
   col  = n - row*(row-1)/2
   a(row, col) = 3^(row-col) * S(row, col)
   with S(n,k) = the Stirling numbers of the second kind.
   (Equation: a(n,m) = 3^(n-m) * stirling2(n,m), a(n,m)=3*m*a(n-1,m)+a(n-1,m-1).) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow3.h"   /* provides seq_t pow3(idx_t exp) */

seq_t a(idx_t n)
{
    /* Determine the row r for this index n (row starts at 1). */
    idx_t r = 1;
    unsigned __int128 cumulative = 1;          /* sum of 1..r  */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* First index of the computed row */
    idx_t start_of_row = (unsigned long long)(cumulative - r + 1);
    /* Column index within the row (1‑based) */
    idx_t c = n - start_of_row + 1;

    /* Build Stirling numbers of the second kind up to row r.
       Only two rows are kept at any time. */
    seq_t *prev = (seq_t *)calloc(r + 1, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(r + 1, sizeof(seq_t));
    if (!prev || !curr) {   /* allocation failure – return 0 as placeholder */
        free(prev); free(curr);
        return 0;
    }

    /* Initial row: S(1,1) = 1 */
    prev[1] = 1;

    for (idx_t row = 2; row <= r; ++row) {
        for (idx_t k = 1; k <= row; ++k) {
            /* S(row,k) = S(row-1,k-1) + k*S(row-1,k) */
            seq_t part1 = prev[k - 1];
            seq_t part2 = prev[k] * (seq_t)k;   /* multiply by small integer k */
            curr[k] = part1 + part2;
        }
        /* Prepare for next iteration: prev <-> curr */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t stirling = prev[c];
    seq_t scale = pow3(r - c);
    seq_t result = stirling * scale;

    free(prev);
    free(curr);
    return result;
}
