/*
 * A205396  Least s(k) such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=ceiling(j^2/2).
 * https://oeis.org/A205396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least s(k) such that * n divides (s(k)-s(j)) for some
 *            j<k, * where s(j) = ceil(j^2/2) = (j*j+1)/2
 * Range:     u8 n <= 33, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* n >= 1 */
    /* Allocate array of first‑occurrence indices for each remainder mod n.
       Use 0 as a sentinel meaning “not yet seen”.  Indices stored are
       1‑based (the j or k value).  The remainder 0 … n-1 fits in the array. */
    idx_t *seen = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!seen) return 0;                      /* unlikely allocation failure */

    for (idx_t i = 0; i < n; ++i)
        seen[i] = 0;                           /* sentinel */

    /* First term j = 1 */
    seq_t s1 = ((seq_t)1 * 1 + 1) / 2;          /* (1*1+1)/2 = 1 */
    idx_t r1 = (idx_t)(s1 % n);
    seen[r1] = 1;                              /* record first occurrence */

    seq_t result = 0;

    /* Search for the smallest k > 1 that repeats a remainder. */
    for (idx_t k = 2;; ++k)
    {
        seq_t sq = (seq_t)k * k;                /* k^2, cast to seq_t */
        seq_t sk = (sq + 1) / 2;                /* ceil(k^2/2) */
        idx_t rk = (idx_t)(sk % n);

        if (seen[rk] != 0)                     /* remainder seen before */
        {
            result = sk;                       /* s(k) is the answer */
            break;
        }
        seen[rk] = k;                          /* record first appearance */
    }

    free(seen);
    return result;
}
