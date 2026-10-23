/*
 * A142963  Triangle read by rows, coefficients of the polynomials P(k, x) =
 *          (1/2) Sum_{p=0..k-1} Stirling2(k,
 *          p+1)*x^p*(1-4*x)^(k-1-p)*(2*p+2)!/(p+1)!.
 * https://oeis.org/A142963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 60, u64 n <= 161,
 *            u128 n <= 210, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A142963
 *  Triangle read by rows, coefficients of the polynomials
 *  P(k, x) = (½) Σ_{p=0}^{k-1} Stirling2(k, p+1)*x^p*(1-4x)^{k-1-p}*(2p+2)!/(p+1)!.
 *
 *  The triangle satisfies                     T(n,0)=1,   T(n,n-1)=2^{n-1}
 *  and for 0< m < n-1:  T(n,m) = (m+1)*T(n-1,m) + (4n-4m-2)*T(n-1,m-1).
 *  The OEIS sequence a(n) is the flattening of this triangle by rows.
 *
 *  Example rows:          1
 *                        1  2
 *                        1 10  4
 *                        1 30 72  8
 *                        ...
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find the row r (1‑based) containing n and number of terms before it (cum). */
    idx_t r = 1;
    idx_t cum = 0;
    while (cum + r < n) {
        cum += r;
        ++r;
    }
    idx_t pos = n - cum - 1;          /* column within the row (0‑based) */

    /* Allocate two rows of size r+1.  The +1 is to allow direct indexing. */
#if defined(__cplusplus)
    seq_t *prev = new seq_t[r + 1]();
    seq_t *curr = new seq_t[r + 1]();
#else
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * (r + 1));
    seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * (r + 1));
    if (!prev || !curr) { abort(); }
#endif

    seq_t result = 0;

    /* Build rows 1 … r (0‑based: i = 0 … r-1). */
    for (idx_t i = 0; i < r; ++i) {
        /* Compute row i+1 */
        if (i == 0) {
            curr[0] = 1;
        } else {
            curr[0] = 1;
            for (idx_t j = 1; j < i; ++j) {
                seq_t left  = ((seq_t)(j + 1)) * prev[j];
                seq_t right = ((seq_t)(4 * (i + 1) - 4 * j - 2)) * prev[j - 1];
                curr[j] = left + right;
            }
            curr[i] = pow2((idx_t)i);  /* 2^i */
        }

        if (i == r - 1) {            /* target row reached */
            result = curr[pos];
            break;
        }

        /* Prepare for next row: copy current row into prev. */
        for (idx_t k = 0; k <= i; ++k) {
            prev[k] = curr[k];
        }
    }

    /* Clean up */
#if defined(__cplusplus)
    delete[] prev;
    delete[] curr;
#else
    free(prev);
    free(curr);
#endif

    return result;
}
