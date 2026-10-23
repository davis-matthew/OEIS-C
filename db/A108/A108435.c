/*
 * A108435  Triangle read by rows: T(n,k) is number of paths from (0,0) to
 *          (3n,0) that stay in the first quadrant (but may touch the
 *          horizontal axis), consisting of steps u=(2,1),U=(1,2), or d=(1,-1)
 *          and have k returns to the x-axis.
 * https://oeis.org/A108435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, pos) where * T(r, k) = (k/(r-k)) * [ 3·2^k·C(r−1,
 *            k) * + Σ_{j=0}^{r−k−2} 2^{r−1−j} * ·(5r−2k+j+1)·C(r−1,
 *            j)·C(2r−k−1, r+j)/(r+j+1) ], * and T(r, r) = 2^r. * This is the
 *            definition of OEIS A108435
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

static seq_t compute_T(idx_t r, idx_t k)
{
    /* T(r, r) = 2^r */
    if (k == r)
        return pow2(r);

    seq_t num = (seq_t)k;                 /* numerator */
    seq_t den = (seq_t)(r - k);           /* denominator, r > k here */

    /* initial term */
    seq_t term = (seq_t)3 * pow2(k) * binom(r - 1, k);

    if (k <= r - 2) {
        idx_t maxj = r - k - 2;
        for (idx_t j = 0; j <= maxj; ++j) {
            seq_t p = pow2(r - 1 - j);                    /* 2^{r-1-j} */
            seq_t coeff = (seq_t)5 * r - (seq_t)2 * k + j + 1;  /* 5r-2k+j+1 */
            p *= coeff;                                   /* 2^{r-1-j}*(5r-2k+j+1) */
            p *= binom(r - 1, j);                         /* *C(r-1, j) */
            p *= binom(2 * r - k - 1, r + j);             /* *C(2r-k-1, r+j) */
            p /= (r + j + 1);                             /* / (r+j+1) */
            term += p;
        }
    }

    /* Final division by (r-k) after multiplying by k */
    return (num * term) / den;
}

seq_t a(idx_t n)
{
    /* Map linear index n (1‑based) to (row r, position k). */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;
    idx_t prev = r * (r - 1) / 2;
    idx_t k = n - prev;                 /* 1 ≤ k ≤ r */
    return compute_T(r, k);
}
