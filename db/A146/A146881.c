/*
 * A146881  A symmetrical triangle sequence of coefficients : p(x,n)=If[n ==
 *          0, 1, (x + 1)^n + Sum[(1 + Mod[Binomial[n, m], 4])*x^m*(1 + x^(n -
 *          2*m)), {m, 1, n - 1}]].
 * https://oeis.org/A146881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^k in the polynomial p(x,r) = (x+1)^r +
 *            Σ_{m=1}^{r-1} (1+Mod[Binomial[r,m],4])·x^m·(1+x^{r-2m}), where r
 *            is the unique integer with r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k = n
 *            - r(r+1)/2. For 0 ≤ k ≤ r: a(n) = 1 if k==0 or k==r, otherwise
 *            a(n) = Binomial[r,k] + 2·(1 + Binomial[r,...
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* find the row r that contains the (flattened) index n */
    idx_t r = 0;
    idx_t offset = 0;
    while (n >= offset + r + 1) {
        offset += r + 1;
        r++;
    }
    idx_t k = n - offset;  /* position inside the row [0..r] */

    if (k == 0 || k == r) {
        return (seq_t)1;
    }

    seq_t bin = binom(r, k);          /* C(r,k) */
    seq_t mod4 = bin & (seq_t)3;      /* C(r,k) mod 4 */
    seq_t coeff = bin + ((seq_t)2) * ((seq_t)1 + mod4);
    return coeff;
}
