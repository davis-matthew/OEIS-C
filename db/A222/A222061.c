/*
 * A222061  Triangle read by rows: coefficients of second-order
 *          hypergeometric-harmonic polynomials.
 * https://oeis.org/A222061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row(n), k(n)) where * T(n,k) = Stirling2(n,k) * k! *
 *            H2(k), * H2(k) = Σ_{i=1..k} (k-i+1)/i. * The triangle is read
 *            row by row starting with a(0) = T(0,0) = 0. * The sequence uses
 *            0‑indexed offset
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   K_array, factorial_array, row_and_k, stirling2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/K_array.h"
#include "../utils/factorial_array.h"
#include "../utils/row_and_k.h"
#include "../utils/stirling2.h"

seq_t a(idx_t n)
{
    /* Find the row 'r' and column 'k' corresponding to linear index n */
    idx_t r, k;
    row_and_k(n, &r, &k);

    /* For k == 0, T(r,0) = 0 by definition (H2(0)=0).              */
    if (k == 0) return (seq_t)0;

    /* Precompute factorials up to 'k' as they are needed for K(k).     */
    seq_t *fac = factorial_array(k);
    seq_t *K   = K_array(k, &fac);

    /* Compute Stirling2(r,k).                                         */
    seq_t S = stirling2(r, k, r, k);

    /* Final result: Stirling2 * K(k).                                */
    seq_t result = S * K[k];

    /* Free temporaries.                                              */
    free(fac);
    free(K);

    return result;
}
