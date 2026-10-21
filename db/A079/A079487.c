/*
 * A079487  Triangle read by rows giving Whitney numbers T(n,k) of Fibonacci
 *          lattices.
 * https://oeis.org/A079487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) gives the flattened triangle of Whitney numbers T(n,k) for
 *            Fibonacci lattices. The rows are defined by polynomials p(k,x):
 *            p(0,x) = 1 if k odd: p(k,x) = x*p(k-1,x) + p(k-2,x) if k even:
 *            p(k,x) = p(k-1,x) + x^2*p(k-2,x) The flattening order is
 *            row‑by‑row: a(0)=p(0,0), a(1)=p(1,0), a(2)=p(1,1), …
 * Range:     u8 n <= 141, u16 n <= 416, u32 n <= 1350, u64 n <= 4896,
 *            u128 n <= 9869, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#if defined(__cplusplus)
    // In C++ use new/delete to construct/ destruct seq_t objects.
    #include <new>
#endif

#if defined(__cplusplus)
static seq_t *alloc_seq_arr(idx_t len)
{
    return new seq_t[len];
}
static void free_seq_arr(seq_t *ptr)
{
    delete[] ptr;
}
#else
static seq_t *alloc_seq_arr(idx_t len)
{
    return (seq_t *)calloc(len, sizeof(seq_t));
}
static void free_seq_arr(seq_t *ptr)
{
    free(ptr);
}
#endif

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column position within the row. */
    idx_t start = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t k = n - start;

    /* Base cases: row 0 and row 1 all coefficients are 1. */
    if (r == 0 || r == 1)
        return (seq_t)1;

    /* Allocate three rows of maximum needed length (r+1). */
    idx_t len = r + 1;
    seq_t *row_prev2 = alloc_seq_arr(len); /* p_{i-2} */
    seq_t *row_prev1 = alloc_seq_arr(len); /* p_{i-1} */
    seq_t *row_curr  = alloc_seq_arr(len); /* current p_i */

    if (!row_prev2 || !row_prev1 || !row_curr)
    {
        free_seq_arr(row_prev2);
        free_seq_arr(row_prev1);
        free_seq_arr(row_curr);
        return (seq_t)0; /* fallback */
    }

    /* Initialize p_0 and p_1. */
    row_prev2[0] = (seq_t)1;          /* p_0 has one coefficient 1 */
    row_prev1[0] = (seq_t)1;          /* constant term of p_1 */
    row_prev1[1] = (seq_t)1;          /* x coefficient of p_1 */

    /* Compute successive polynomials up to row r. */
    for (idx_t i = 2; i <= r; ++i)
    {
        /* Zero out current row up to degree i. */
        for (idx_t j = 0; j <= i; ++j)
            row_curr[j] = (seq_t)0;

        if (i & 1)   /* odd: p_i = x*p_{i-1} + p_{i-2} */
        {
            /* x*p_{i-1} shifts coefficients up by 1. */
            for (idx_t j = 1; j <= i; ++j)
                row_curr[j] += row_prev1[j-1];
            /* + p_{i-2} */
            for (idx_t j = 0; j <= i-2; ++j)
                row_curr[j] += row_prev2[j];
        }
        else        /* even: p_i = p_{i-1} + x^2*p_{i-2} */
        {
            /* p_{i-1} */
            for (idx_t j = 0; j <= i-1; ++j)
                row_curr[j] += row_prev1[j];
            /* x^2*p_{i-2} shifts coefficients up by 2. */
            for (idx_t j = 2; j <= i; ++j)
                row_curr[j] += row_prev2[j-2];
        }

        /* Rotate row buffers: p_{i-2} <- p_{i-1}, p_{i-1} <- p_i. */
        seq_t *tmp = row_prev2;
        row_prev2 = row_prev1;
        row_prev1 = row_curr;
        row_curr  = tmp;
    }

    /* The required coefficient is now in row_prev1[k]. */
    seq_t result = row_prev1[k];

    free_seq_arr(row_prev2);
    free_seq_arr(row_prev1);
    free_seq_arr(row_curr);

    return result;
}
