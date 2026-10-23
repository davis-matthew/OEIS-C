/*
 * A108086  Triangle, read by rows, where T(0,0) = 1, T(n,k) =
 *          (-1)^(n+k)*T(n-1,k) + T(n-1,k-1); a signed version of Pascal's
 *          triangle.
 * https://oeis.org/A108086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(0,0)=1; T(n,k)=(-1)^(n+k)*T(n-1,k)+T(n-1,k-1) for 0≤k≤n
 * Range:     i8 n <= 58, i16 n <= 178, i32 n <= 610, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* The types seq_t and idx_t are defined by the build system.
   No redefinition is needed. */

/* Return T(r,c) of the signed Pascal triangle. */
static seq_t compute_T(idx_t r, idx_t c)
{
    if (r == 0)      /* only element is 1 */
        return (seq_t)1;

    /* Allocate two rows of size r+1. */
    idx_t sz = r + 1;
    seq_t *prev = (seq_t *)calloc((size_t)sz, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc((size_t)sz, sizeof(seq_t));
    if (!prev || !cur) {
        free(prev);
        free(cur);
        return (seq_t)0;               /* allocation failed – unlikely */
    }

    /* row 0 */
    prev[0] = (seq_t)1;

    for (idx_t n = 1; n <= r; ++n) {
        /* k = 0 */
        cur[0] = ((n & 1) ? -(seq_t)1 : (seq_t)1) * prev[0];

        /* middle positions */
        for (idx_t k = 1; k < n; ++k) {
            seq_t sign = ((n + k) & 1) ? -(seq_t)1 : (seq_t)1;   /* (-1)^(n+k) */
            cur[k] = sign * prev[k] + prev[k - 1];
        }

        /* k = n */
        cur[n] = (seq_t)1;             /* T(n,n) = 1 */

        /* swap rows for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    seq_t result = prev[c];

    free(prev);
    free(cur);
    return result;
}

/* External entry point required by the harness. */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    /* Use long double for robustness. */
    long double tmp = sqrtl((long double)8 * n + 1);
    idx_t r = (idx_t)((tmp - 1) / 2);

    /* Correct possible rounding errors. */
    if ( (r + 1) * (r + 2) / 2 <= n )
        ++r;
    else if ( r * (r + 1) / 2 > n )
        --r;

    idx_t c = n - r * (r + 1) / 2;

    return compute_T(r, c);
}
