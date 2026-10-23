/*
 * A103407  Triangle of absolute values of the coefficients (in descending
 *          powers) of the characteristic polynomials of n X n matrices with
 *          3's on the main diagonal and 1's elsewhere.
 * https://oeis.org/A103407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r = row_of(n) (so r(r+1)/2 ≤ n < (r+1)(r+2)/2). Let k = n -
 *            r(r+1)/2 (0 ≤ k ≤ r). Then a(n) = 1 if k = 0 a(n) =
 *            (r+2)·2^(r-1) if k = r a(n) = (2·C(r-1,r-k-1)+(r+2)·C(r-1,r-k))
 *            · 2^(k-1) otherwise where C(n,k)=binomial coefficient
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binom_small, pow2, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/row_of.h"
#include "../utils/binom_small.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Determine the row r for index n */
    idx_t r = row_of(n);                 /* r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t start = (r * (r + 1)) / 2;
    idx_t k     = n - start;             /* position in row, 0 ≤ k ≤ r */

    /* Leading coefficient */
    if (k == 0) {
        return (seq_t)1;
    }

    /* Constant term (k = r) */
    if (k == r) {
        seq_t base  = (seq_t)(r + 2);          /* r+2 */
        seq_t pow   = pow2(r - 1);             /* 2^(r-1) */
        return base * pow;
    }

    /* Middle coefficients (1 ≤ k ≤ r-1) */
    /* Use k and r to avoid large intermediate exponents */
    idx_t c1_k = r - k - 1;                   /* choose index for first binomial */
    idx_t c2_k = r - k;                       /* choose index for second binomial */
    seq_t comb1  = binom_small(r - 1, c1_k);  /* C(r-1, r-k-1) */
    seq_t comb2  = binom_small(r - 1, c2_k);  /* C(r-1, r-k)   */

    seq_t val    = comb1 * ((seq_t)2) + comb2 * ((seq_t)(r + 2));
    seq_t shift  = pow2(k - 1);               /* 2^(k-1) */

    return val * shift;
}
