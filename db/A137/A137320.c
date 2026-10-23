/*
 * A137320  Coefficients of raising factorial polynomials, T(n,k) = [x^k] p(x,
 *          n) where p(x, n) = (m*x + n - 1)*p(x, n - 1) with p[x, 0] = 1,
 *          p[x, -1] = 0, p[x, 1] = m*x and m = 2. Triangle read by rows, for
 *          n >= 0 and 0 <= k <= n.
 * https://oeis.org/A137320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 38, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A137320
 *
 *  a(n) = [x^k] ∏_{i=1..r} (2x + i – 1)
 *
 *  where
 *      r = ⌊(√(8·n+1)–1)/2⌋   (row number)
 *      k = n – r·(r+1)/2       (column in that row)
 *
 *  The recurrence for the triangle T(n,k) is
 *      T(0,0) = 1
 *      T(n,k) = (n–1)·T(n–1,k) + 2·T(n–1, k–1) for n>0, 0≤k≤n
 *  (with T(n,–1)=T(n,n+1)=0).  a(n) is the flattening of T by rows.
 *
 *  The implementation below computes the required row from scratch
 *  for each call.  For n up to ~10 000 this is well below the
 *  2‑second limit.
 */

#include <stdlib.h>
#include <stddef.h>

/* Auxiliary: triangular number of r, using 128‑bit to avoid overflow */
static inline idx_t triangular(idx_t r)
{
    return (idx_t)(((__uint128_t)r * (r + 1)) / 2);
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Determine row r and column k for index n */
    idx_t r = 0;
    while (triangular(r + 1) <= n)
        ++r;
    idx_t k = n - triangular(r);

    /* Row 0 is trivial */
    if (r == 0)
        return (seq_t)1;

    /* Allocate two buffers of size r+1 for the DP */
    seq_t *prev = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!prev)
        return (seq_t)0;   /* allocation failure */

    seq_t *cur  = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!cur) {
        free(prev);
        return (seq_t)0;
    }

    /* Initial row 0 */
    prev[0] = (seq_t)1;

    /* Build rows 1 .. r */
    for (idx_t i = 1; i <= r; ++i) {
        /* first element */
        cur[0] = (seq_t)(i - 1) * prev[0];

        /* middle elements */
        for (idx_t j = 1; j < i; ++j) {
            cur[j] = (seq_t)(i - 1) * prev[j] + (seq_t)2 * prev[j - 1];
        }

        /* last element */
        cur[i] = (seq_t)2 * prev[i - 1];

        /* swap buffers for next iteration */
        seq_t *tmp  = prev;
        prev = cur;
        cur  = tmp;
    }

    /* The desired coefficient is in prev[k] */
    seq_t result = prev[k];

    /* Clean up */
    free(prev);
    free(cur);

    return result;
}
