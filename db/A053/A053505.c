/*
 * A053505  Number of degree-n permutations of order dividing 30.
 * https://oeis.org/A053505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(n) = Σ_{d ∈ {1,2,3,5,6,10,15,30}, d ≤ n}
           (n‑1)!/(n‑d)! * a(n‑d)
   i.e. sum over allowed cycle lengths of the factorial ratio times
   the partial counts. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Memoisation array */
    static seq_t *A   = NULL;
    static size_t  sz = 0;   /* allocated size (elements [0 .. sz-1]) */

    /* Allowed cycle lengths for permutations of order dividing 30 */
    static const idx_t D[] = {1,2,3,5,6,10,15,30};
    static const size_t Dcnt = sizeof(D)/sizeof(D[0]);

    /* Ensure we have storage for indices up to n */
    if (n >= sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(A, new_sz * sizeof(seq_t));
        if (!tmp) {          /* allocation failed – give up */
            if (A) free(A);
            A = NULL;
            sz = 0;
            return 0;
        }
        /* Zero the newly allocated part */
        if (new_sz > sz)
            memset(tmp + sz, 0, (new_sz - sz) * sizeof(seq_t));
        A = tmp;
        sz = new_sz;
    }

    /* Base case */
    if (n == 0) {
        if (A[0] == 0) A[0] = (seq_t)1;
        return A[0];
    }

    /* Compute all required values up to n when necessary */
    for (idx_t i = 1; i <= n; ++i) {
        if (A[i] != (seq_t)0) continue;   /* already computed */

        seq_t sum = (seq_t)0;
        for (size_t j = 0; j < Dcnt; ++j) {
            idx_t k = D[j];
            if (k > i) continue;
            /* compute (i‑1)! / (i‑k)! = product_{t=1}^{k‑1} (i‑t) */
            seq_t prod = (seq_t)1;
            for (idx_t t = 1; t < k; ++t)
                prod *= (seq_t)(i - t);
            sum += prod * A[i - k];
        }
        A[i] = sum;
    }

    return A[n];
}
