/*
 * A132186  Number of idempotent n X n matrices over GF(2); also number of
 *          diagonalizable n X n matrices over GF(2).
 * https://oeis.org/A132186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} 2^{k(n−k)} · [n,k]_2 where [n,k]_2 is the
 *            Gaussian binomial coefficient with q=2. The implementation
 *            builds each row of the q‑binomial triangle iteratively, using
 *            the recurrence [i,k] = [i−1,k] + 2^{i−k} · [i−1,k−1]. The
 *            sequence value a(n) is then the sum over the current row w...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   shift_by
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/shift_by.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate two rows of size n+1 (0‑based indices). */
    seq_t *prev = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!prev || !cur) {
        /* Memory allocation failed; return 0 as a safe fallback. */
        free(prev);
        free(cur);
        return (seq_t)0;
    }

    prev[0] = (seq_t)1;                /* [0,0] = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        cur[0] = (seq_t)1;              /* [i,0] = 1 */

        /* Build row [i,k] for k = 1..i using the recurrence. */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term1 = prev[k];                           /* [i-1,k]  */
            seq_t term2 = shift_by(prev[k - 1], i - k);      /* 2^{i-k}·[i-1,k-1] */
            cur[k] = term1 + term2;
        }

        /* Compute a(i) = Σ_{k=0..i} 2^{k(i-k)} · cur[k] */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= i; ++k) {
            unsigned long long exp = (unsigned long long)k * (i - k);
            seq_t t = shift_by(cur[k], exp);                 /* 2^{k(i-k)}·cur[k] */
            sum += t;
        }

        if (i == n) {
            free(prev);
            free(cur);
            return sum;   /* final result */
        }

        /* Swap rows for the next iteration. */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* The function should never reach here. */
    free(prev);
    free(cur);
    return (seq_t)0;
}
