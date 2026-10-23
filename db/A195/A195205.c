/*
 * A195205  Triangle of coefficients of a sequence of binomial type
 *          polynomials.
 * https://oeis.org/A195205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = coefficient of x^k in P_m(x) where m(m+1)/2 < n ≤
 *            (m+1)(m+2)/2 and k = n - m(m+1)/2. Here P_0(x)=1 and P_{t+1}(x)
 *            = x*(P_t(x)+2*P_t(x+1)) (recurrence provided in the OEIS entry)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for dynamic allocation and binomial helper */
#include <stdlib.h>
/* Assuming binom(idx_t a, idx_t b) is available from utils. */
#include "../utils/binom.h"

/* The function as required by the build system. */
seq_t a(idx_t n)
{
    /* For n <= 0 we return 0 (unused index). */
    if (n == 0)
        return 0;

    /* Find row m (1‑based) and position k in that row. */
    idx_t m = 0;
    while ((m * (m + 1)) / 2 < n)
        ++m;
    idx_t prev_tri = m * (m - 1) / 2;
    idx_t k = n - prev_tri;      /* 1‑based column in row m */

    /* Build polynomials up to row m. */
    /* Start with P_0(x) = 1. */
    idx_t curr_size = 1;                 /* size of current coefficient array = degree + 1 */
    seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * curr_size);
    if (!curr) return 0;                 /* allocation failed */
    curr[0] = 1;                         /* P_0(x) coefficients: only a_0 = 1 */

    for (idx_t row = 0; row < m; ++row)
    {
        /* Next polynomial will have size curr_size+1 (degree increases by 1). */
        idx_t next_size = curr_size + 1;
        seq_t *next = (seq_t *)calloc(next_size, sizeof(seq_t));
        if (!next) { free(curr); return 0; }

        /* Compute shift polynomial: P_row(x+1) coefficients. */
        /* shift[j] = sum_{i=j}^{curr_size-1} curr[i] * C(i,j)  for 0 <= j < curr_size */
        seq_t *shift = (seq_t *)calloc(curr_size, sizeof(seq_t));
        if (!shift) { free(curr); free(next); return 0; }

        for (idx_t i = 0; i < curr_size; ++i)
        {
            for (idx_t j = 0; j <= i; ++j)
            {
                /* binom(i, j) */
                seq_t b = binom((idx_t)i, (idx_t)j);
                shift[j] = shift[j] + curr[i] * b;
            }
        }

        /* Compute the next polynomial: P_{row+1}(x) = x*(curr + 2*shift). */
        for (idx_t j = 0; j < curr_size; ++j)
        {
            seq_t val = curr[j] + (seq_t)2 * shift[j];
            next[j + 1] = val;       /* multiply by x shifts degree up by 1 */
        }

        /* Clean up and prepare for next iteration. */
        free(shift);
        free(curr);
        curr = next;
        curr_size = next_size;
    }

    /* At this point 'curr' holds coefficients of P_m(x).   curr[0] is the constant term (0). */
    seq_t result = curr[k];   /* k is 1‑based column index, 0‑based array index is k */
    free(curr);
    return result;
}
