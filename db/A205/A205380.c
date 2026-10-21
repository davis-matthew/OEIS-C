/*
 * A205380  Least s(k) such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=(2j-1)^2.
 * https://oeis.org/A205380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least odd square s(k) = (2k‑1)^2 such that
   n divides s(k) – s(j) for some j < k (OEIS A205380).
   The algorithm stores the residues of odd squares modulo n.
   The first repeated residue indicates that n divides the
   difference of the corresponding squares, and the current
   square is the minimal one satisfying the condition. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)    /* undefined for n = 0, guard against bad input */
        return 0;

    /* Allocate a boolean array to record which residues have appeared. */
    uint8_t *visited = (uint8_t *)calloc((size_t)n, sizeof(uint8_t));
    if (!visited)
        return 0;   /* allocation failure */

    for (idx_t k = 1;; ++k) {
        idx_t odd = 2 * k - 1;                 /* current odd integer */
        seq_t odd_sq = (seq_t)odd * (seq_t)odd; /* (2k−1)^2 */
        idx_t residue = (idx_t)(odd_sq % (seq_t)n);

        if (visited[residue]) {                /* first repeat found */
            free(visited);
            return odd_sq;                     /* minimal s(k) */
        }
        visited[residue] = 1;
    }
}
