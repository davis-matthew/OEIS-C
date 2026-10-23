/*
 * A146771  Triangle read by rows: expansion of p(x,n)=If[n == 0, 1, (x + 1)^n
 *          + 2^(n-2)*Sum[Binomial[n-m, m]*x^m*(1 + x^(n - 2*m)), {m, 1, n -
 *          1}]].
 * https://oeis.org/A146771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   T(n,k) = binomial(n,k) + 2^(n-2) * ( binomial(n-k,k) +
 *            binomial(k,n-k) ) for 1 ≤ k ≤ n-1 a(n) is the coefficients of
 *            p(x,n) flattened row by row. The function computes the
 *            coefficient at position n in that flattening
 * Range:     u8 n <= 29, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    idx_t offset = r * (r + 1) / 2;
    idx_t j = n - offset;                /* column index within the row */

    /* Allocate an array for the row's coefficients */
    seq_t *coeff = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!coeff) return 0;                /* fallback on allocation failure */

    /* Initial terms from (x + 1)^r */
    for (idx_t k = 0; k <= r; ++k) {
        coeff[k] = binom(r, k);
    }

    /* Add the additional terms for r ≥ 2 */
    if (r >= 2) {
        seq_t power = pow2(r - 2);       /* 2^(r-2) */
        for (idx_t m = 1; m <= r - 1; ++m) {
            seq_t bin = binom(r - m, m);
            seq_t contrib = power * bin; /* exact product */
            coeff[m] += contrib;
            coeff[r - m] += contrib;
        }
    }

    seq_t result = coeff[j];
    free(coeff);
    return result;
}
