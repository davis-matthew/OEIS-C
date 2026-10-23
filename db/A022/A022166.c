/*
 * A022166  Triangle of Gaussian binomial coefficients (or q-binomial
 *          coefficients) [n,k] for q = 2.
 * https://oeis.org/A022166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T is the q‑binomial coefficient * with q = 2
 *            (Gaussian binomial), flattened row‑major. * r is the row index
 *            such that r*(r+1)/2 <= n < (r+1)*(r+2)/2, * k = n - r*(r+1)/2. *
 *            Uses the recurrence * T(0,0) = 1 * T(n,k) = T(n-1,k-1) + 2^k *
 *            T(n-1,k) (0 < k < n) * T(n,0) = T(n,n) = 1
 * Range:     u8 n <= 22, u16 n <= 38, u32 n <= 81, u64 n <= 142,
 *            u128 n <= 285, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stddef.h>

#if defined(__cplusplus)
#  define ALLOC_ROW(sz)  (new seq_t[(sz)])
#  define FREE_ROW(p)    (delete[] (p))
#else
#  define ALLOC_ROW(sz)  ((seq_t*)malloc((sz)*sizeof(seq_t)))
#  define FREE_ROW(p)    (free(p))
#endif

seq_t a(idx_t n)
{
    /* Find row r such that T(r,0..r) contains entry n */
    double d = sqrt((double)8 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* Correct possible rounding errors */
    while (r * (r + 1) / 2 > n) --r;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Column index within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Trivial case */
    if (r == 0)
        return (seq_t)1;

    /* Allocate arrays for previous and current rows */
    seq_t *prev = ALLOC_ROW(r + 1);
    if (!prev) return (seq_t)0;  /* allocation failed */

    /* Base row 0 */
    prev[0] = (seq_t)1;

    /* Build rows 1 .. r */
    for (idx_t i = 1; i <= r; ++i) {
        seq_t *cur = ALLOC_ROW(i + 1);
        if (!cur) { FREE_ROW(prev); return (seq_t)0; }

        cur[0] = (seq_t)1;
        cur[i] = (seq_t)1;

        seq_t pow2 = (seq_t)2;  /* 2^1 */
        for (idx_t j = 1; j < i; ++j) {
            cur[j] = prev[j-1] + pow2 * prev[j];
            pow2   = pow2 * (seq_t)2;
        }

        /* Next iteration uses this row as previous */
        FREE_ROW(prev);
        prev = cur;
    }

    /* Result is the k-th element of the r-th row */
    seq_t result = prev[k];
    FREE_ROW(prev);
    return result;
}
