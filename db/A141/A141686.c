/*
 * A141686  Triangle read by rows: T(n, k) = binomial(n-1, k-1)*A008292(n, k).
 * https://oeis.org/A141686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) term of the triangle read by rows where the
 *            r‑th row is defined by T(r,k) = binomial(r-1, k-1) *
 *            A008292(r,k) for 1 ≤ k ≤ r. The sequence a(n) is the flattening
 *            of this triangle in row‑major order (offset 1). The function
 *            below computes a(n) exactly for any build (uint8_t … arb...
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 70, u64 n <= 160,
 *            u128 n <= 447, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"          /* seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    if (n < 1)               /* undefined for n<1, return 0 safely */
        return (seq_t)0;

    /* Find the row r such that the first r*(r+1)/2 terms include n. */
    idx_t r   = 1;
    idx_t cum = 1;           /* total terms up to row r */
    while (cum < n) {
        r++;
        cum += r;
    }

    /* Position k within the found row (1‑based). */
    idx_t terms_before = cum - r;   /* number of terms before row r */
    idx_t k = n - terms_before;     /* 1 ≤ k ≤ r */

    /* Allocate temporary arrays for computing Eulerian numbers A008292. */
    seq_t *prev = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Base case: A(1,1) = 1. */
    prev[1] = (seq_t)1;

    /* Compute rows 2..r of the Eulerian triangle. */
    for (idx_t i = 2; i <= r; i++) {
        curr[1] = (seq_t)1;
        curr[i] = (seq_t)1;
        for (idx_t j = 2; j <= i - 1; j++) {
            seq_t term1 = (seq_t)j * prev[j];
            seq_t term2 = (seq_t)(i - j + 1) * prev[j - 1];
            curr[j] = term1 + term2;
        }
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Now prev[k] is A008292(r,k); compute the binomial factor. */
    seq_t eulerian = prev[k];
    seq_t comb     = binom(r - 1, k - 1);

    seq_t result = comb * eulerian;

    free(prev);
    free(curr);
    return result;
}
