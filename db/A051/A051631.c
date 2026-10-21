/*
 * A051631  Triangle formed using Pascal's rule except begin and end n-th row
 *          with n-1.
 * https://oeis.org/A051631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, sign, tabl
 * Formula:   a(n) = T(r,k) where T(0,0) = -1, T(m,0) = T(m,m) = m‑1, T(m,j) =
 *            T(m‑1,j) + T(m‑1,j‑1) for 0<j<m. For a given n, let r =
 *            floor((√(8n+1)–1)/2) and k = n – r(r+1)/2. Then a(n) = T(r,k)
 * Range:     i8 n <= 57, i16 n <= 178, i32 n <= 643, i64 n <= 2307,
 *            i128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <cstddef>
#endif

static inline idx_t triangular_rowsum(idx_t r)
{
    /* number of elements up to and including row r */
    unsigned __int128 t = (unsigned __int128)(r + 1) * (r + 2);
    return (idx_t)(t >> 1);
}

seq_t a(idx_t n)
{
    /* Determine the row r that contains index n */
    idx_t r = 0;
    while (triangular_rowsum(r) <= n)
        ++r;

    /* k = index within the row (0‑based) */
    idx_t prev_sum = (unsigned __int128)r * (r + 1) >> 1; /* sum up to row r‑1 */
    idx_t k = n - prev_sum;

    if (r == 0)   /* only the single element –1 */
        return (seq_t)(-1);

    /* Allocate two rows of length at most r+1 */
#ifdef __cplusplus
    seq_t *prev = new seq_t[r + 1];
    seq_t *curr = new seq_t[r + 1];
#else
    seq_t *prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((r + 1) * sizeof(seq_t));
#endif

    /* Row 0 */
    prev[0] = (seq_t)(-1);

    /* Build rows 1 … r */
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = (seq_t)(i - 1);        /* left boundary */
        curr[i] = (seq_t)(i - 1);        /* right boundary */
        for (idx_t j = 1; j < i; ++j)
            curr[j] = prev[j] + prev[j - 1];
        /* swap prev and curr pointers for next iteration */
        seq_t *tmp = prev; prev = curr; curr = tmp;
    }

    seq_t result = prev[k];

#ifndef __cplusplus
    free(prev);
    free(curr);
#else
    delete[] prev;
    delete[] curr;
#endif

    return result;
}
