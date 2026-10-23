/*
 * A047172  Number of nonempty subsets of {1,2,...,n} in which exactly 1/3 of
 *          the elements are <= (n-1)/2.
 * https://oeis.org/A047172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047172: Number of nonempty subsets of {1,2,…,n} in which exactly 1/3 of the elements are <= (n-1)/2.
   Formula: a(n) = Σ_{t=1}^{Kmax} binomial(L, t) * binomial(U, 2t)
            where L = ⌊(n‑1)/2⌋, U = n – L,
            and Kmax = min(L, ⌊U/2⌋). */

#include <stdint.h>
#include "../utils/binomial.h"   /* provides static seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Lower and upper half sizes */
    idx_t L = (n - 1) / 2;          /* elements <= (n-1)/2 */
    idx_t U = n - L;               /* remaining elements */

    /* Max t such that t <= L and 2t <= U */
    idx_t halfU = U / 2;
    idx_t Kmax = (L < halfU) ? L : halfU;

    seq_t sum = 0;

    for (idx_t t = 1; t <= Kmax; ++t) {
        seq_t low  = binomial(L, t);      /* choose t from the lower half */
        seq_t high = binomial(U, 2 * t);  /* choose 2t from the upper half */
        sum += low * high;
    }

    return sum;
}
