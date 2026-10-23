/*
 * A104698  Triangle read by rows: T(n,k) = Sum_{j=0..n-k} binomial(k,
 *          j)*binomial(n-j+1, k+1).
 * https://oeis.org/A104698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,c) where r,c are obtained from the flattened index n
 *            by solving r(r+1)/2 ≤ n < (r+1)(r+2)/2, with c = n – r(r+1)/2.
 *            The triangle entries are given by T(r,c) = Σ_{j=0}^{r-c}
 *            binomial(c,j) * binomial(r-j+1, c+1). (For the boundary cases,
 *            T(r,0)=r+1 and T(r,r)=1.)
 * Range:     u8 n <= 30, u16 n <= 110, u32 n <= 389, u64 n <= 1453,
 *            u128 n <= 5049, big any n
 * Helpers:   binom, index_to_rc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the numeric utilities that provide `binom`. */
#include <stdint.h>
#include <math.h>
#include "../utils/binom.h"
#include "../utils/index_to_rc.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t r, c;
    index_to_rc(n, &r, &c);

    /* Boundary cases. */
    if (c == 0)
        return (seq_t)r + 1;
    if (c == r)
        return (seq_t)1;

    /* Compute the sum Σ_{j=0}^{r-c} binomial(c,j) * binomial(r-j+1, c+1). */
    seq_t sum = 0;
    idx_t jmax = r - c;
    for (idx_t j = 0; j <= jmax; ++j) {
        seq_t term1 = binom(c, j);
        seq_t term2 = binom(r - j + 1, c + 1);
        sum += term1 * term2;
    }
    return sum;
}
