/*
 * A224168  Number of n X 3 0..3 arrays with rows, diagonals and antidiagonals
 *          unimodal and columns nondecreasing.
 * https://oeis.org/A224168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{min(9,n-1)} c_k * binomial(n+8-k, 9) with
 *            coefficient vector c = {50,184,149,378,-327,412,-228,107,-22,3}
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n<1, return 0 */

    const int coeff[10] = {50,184,149,378,-327,412,-228,107,-22,3};
    idx_t limit = (n <= 10) ? (n - 1) : 9;      /* number of terms to use */

    seq_t pos = 0;   /* sum of positive contributions */
    seq_t neg = 0;   /* sum of absolute values of negative contributions */

    for (idx_t k = 0; k <= limit; ++k) {
        seq_t term = binomial(n + 8 - k, 9);
        int c = coeff[k];
        if (c >= 0)
            pos += (seq_t)c * term;
        else
            neg += (seq_t)(-c) * term;
    }

    return pos - neg;   /* final non‑negative result */
}
