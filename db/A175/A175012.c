/*
 * A175012  Triangle generated from the g.f of A000712 (i.e., 1/(1-x^m)^2)
 *          interleaved with zeros.
 * https://oeis.org/A175012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is obtained by the following construction:
 *   Let S_m(x) = 1 + 2x^m + 3x^(2m) + …  (generating function of A000712 interleaved with zeros)
 *   Define R_k(x) = S_1(x) * S_2(x) * … * S_k(x)   (k ≥ 1)
 *   Let t_0(x) = R_1(x) and for k ≥ 1, t_k(x) = R_(k+1)(x) – R_k(x).
 *   The triangle consists of the coefficient sequences of t_0, t_1, t_2, ….
 *   Finally the flattened sequence a(n) is obtained by reading this triangle column‑wise:
 *   for j = 0,1,2,… output t_{j-1}(j), t_{j-2}(j), …, t_0(j)  (t_{-1} is omitted).
 *   For j = 0 the only term is t_0(0) = 1.
 *
 * The algorithm below turns the description into an efficient O(n) routine for computing
 * a(n) directly for any 0 ≤ n, without storing the whole triangle.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Determine the column j and offset d such that
        n = C(j) + d  where C(j) = 1 + (j-1)j/2  (j≥1) and C(0)=0.
    */
    idx_t j = 0;
    idx_t Cj = 0;          /* cumulative count up to column j */
    if (n > 0) {
        /* Find minimal j with C(j+1) > n */
        while (1) {
            idx_t next_j = j + 1;
            idx_t Cnext = 1 + (next_j - 1) * next_j / 2;  /* C(j+1) */
            if (n < Cnext) break;
            j = next_j;
            Cj = Cnext;
        }
    }

    /* Offset within column j */
    idx_t d = n - Cj;

    /* Row r in column j: ranging from r = j-1 (top) down to r = 0 (bottom) */
    idx_t r = (j == 0) ? 0 : (j - 1) - d;

    /* Base case: r == 0 → simply the coefficient of S_1(x) at index j */
    if (r == 0) {
        return (seq_t)(j + 1);
    }

    /* We need to compute R_k[j] for k = 1 … r+1.
       The recurrence:  R_{k}(x) = R_{k-1}(x) * S_k(x)
       and t_{k-1}(x) = R_k(x) – R_{k-1}(x).
       We keep only the coefficients up to index j.
    */
    size_t m = (size_t)(j + 1);          /* number of coefficients needed */
    seq_t *arr  = (seq_t *)calloc(m, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(m, sizeof(seq_t));

    /* Initialize R_1 = S_1(x) : coefficients are n+1 */
    for (size_t i = 0; i < m; ++i) {
        arr[i] = (seq_t)(i + 1);
    }

    /* Iterate k = 2 … r+1 */
    for (idx_t k = 2; k <= r + 1; ++k) {
        /* Compute next = arr * S_k  (convolution) */
        for (size_t nidx = 0; nidx < m; ++nidx) {
            /* Constant term from m=0 */
            seq_t val = arr[nidx];

            /* Add contributions from m = 1 … ⌊nidx/k⌋  */
            for (idx_t mval = 1; mval * k <= (idx_t)nidx; ++mval) {
                val += arr[nidx - (size_t)(mval * k)] * (seq_t)(mval + 1);
            }
            next[nidx] = val;
        }

        /* If this is the final step we need, compute the requested value */
        if (k == r + 1) {
            seq_t result = next[(size_t)j] - arr[(size_t)j];
            free(arr);
            free(next);
            return result;
        }

        /* Prepare for next iteration: arr = next, reuse buffers */
        seq_t *tmp = arr;
        arr = next;
        next = tmp;
    }

    /* In theory we should never reach here */
    free(arr);
    free(next);
    return 0;
}
