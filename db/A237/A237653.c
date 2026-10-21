/*
 * A237653  O.g.f.: Sum_{n>=0} x^n*Product_{k=1..n} (k + x).
 * https://oeis.org/A237653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A237653
    Formula: a(n) = ∑_{k=1}^{n+1} s(n - k + 2, k)
    where s(m, k) are the unsigned Stirling numbers of the first kind.
    The implementation uses the recurrence
        s(m, k) = s(m-1, k-1) + (m-1)·s(m-1, k)
    and evaluates only the contributions needed for a given n.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* convert the 64‑bit index to a native size_t for loops */
    size_t N = (size_t)n;
    /* maximum row index needed is N+1 */
    size_t max_row = N + 1;
    /* allocate two temporary rows of length N+2 (enough for all rows) */
    size_t alloc_size = N + 2;
    seq_t *prev = (seq_t *)calloc(alloc_size, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(alloc_size, sizeof(seq_t));
    if (prev == NULL || curr == NULL) {
        /* memory allocation failed – return zero (should not happen) */
        return (seq_t)0;
    }
    /* row 0: s(0,0) = 1 */
    prev[0] = (seq_t)1;

    seq_t sum = (seq_t)0;

    /* Build rows 1 … N+1, accumulating the term that contributes to a(N) */
    for (size_t r = 1; r <= max_row; ++r) {
        /* first entry of each row is zero (s(r,0)=0) */
        curr[0] = (seq_t)0;
        for (size_t k = 1; k <= r; ++k) {
            seq_t left  = prev[k - 1];
            seq_t right = (k < r) ? prev[k] : (seq_t)0;   /* s(r-1, k) = 0 if k ≥ r */
            curr[k] = left + (seq_t)(r - 1) * right;       /* recurrence */
        }
        /* Determine if this row contains the entry contributing to a(N) */
        /* Index condition: r + k - 2 == N  →  k == N - r + 2 */
        size_t k_needed = N + 2 - r;
        if (k_needed >= 1 && k_needed <= r) {
            sum += curr[k_needed];
        }
        /* Prepare for next iteration: swap prev and curr */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* clean up */
    free(prev);
    free(curr);

    return sum;
}
