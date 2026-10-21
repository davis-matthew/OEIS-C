/*
 * A219034  Triangular array read by rows: T(n,k) is the number of forests of
 *          rooted trees on n labeled nodes with exactly k isolated nodes;
 *          n>=0, 0<=k<=n.
 * https://oeis.org/A219034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 65, u64 n <= 152,
 *            u128 n <= 405, big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A219034
 * Formula: T(n,k) = A105785(n-k) * C(n,k),
 * where A105785(m) = (m+1)^(m-1) - Σ_{t=1..m} C(m,t) * A105785(m-t).
 * The function a(idx_t n) returns the triangular array read by rows,
 * i.e. a(n) = T(r,k) with r = floor((√(8n+1)-1)/2) and k = n - r(r+1)/2.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper functions provided by the build system */
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, idx_t exp); */

/* Cached values of A105785(m) for m = 0 .. A_count-1 */
static seq_t *A_values = NULL;
static idx_t  A_count  = 0;

/* Ensure that A_values[0..up] are available */
static void ensure_A_up_to(idx_t up)
{
    if (A_count > up)
        return;

    /* Allocate a new array of the required size */
    seq_t *newp = (seq_t *)malloc((size_t)(up + 1) * sizeof(seq_t));
    if (!newp)
        exit(1);                  /* unrecoverable allocation error */

    /* Copy existing values, if any */
    for (idx_t i = 0; i < A_count; ++i)
        newp[i] = A_values[i];

    /* Compute new entries */
    for (idx_t m = A_count; m <= up; ++m) {
        if (m == 0) {
            newp[0] = (seq_t)1;
        } else {
            seq_t total = big_pow((seq_t)(m + 1), (idx_t)(m - 1));
            seq_t sum   = (seq_t)0;
            for (idx_t t = 1; t <= m; ++t) {
                seq_t comb = binomial(m, t);
                seq_t prod = seq_mul(comb, newp[m - t]);   /* m-t < m */
                sum = sum + prod;
            }
            newp[m] = total - sum;   /* total ≥ sum for all m */
        }
    }

    /* Free old storage and replace with the new array */
    free(A_values);
    A_values = newp;
    A_count  = up + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r * (r + 1)) / 2) <= n)
        ++r;
    --r;                                   /* now r satisfies r*(r+1)/2 <= n */
    idx_t start = ((r * (r + 1)) / 2);
    idx_t k     = n - start;               /* column index within row r */

    /* Ensure required A105785 values are computed up to r */
    ensure_A_up_to(r);

    /* Compute T(r,k) = A105785(r-k) * C(r,k) */
    seq_t a_val = A_values[r - k];
    seq_t comb  = binomial(r, k);
    return seq_mul(a_val, comb);
}
