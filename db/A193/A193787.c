/*
 * A193787  Triangular array: the fusion of polynomial sequences P and Q given
 *          by p(n,x)=(x+1)^n and q(n,x)=1+x^n.
 * https://oeis.org/A193787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   binom_n_k, pow2, triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle array: for n ≥ 1, row n consists of the binomial
   coefficients C(n–1,k) for k = 0…n–1 followed by 2^(n–1).
   Row 0 is [1].  a(n) is the linearized, mirrored version of this
   array (rows reversed). */
#include <math.h>
#include "../utils/pow2.h"
#include "../utils/binom_n_k.h"
#include "../utils/triangular.h"

/* Return the n‑th triangular number t_n = n*(n+1)/2.  */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;          /* a(0) = 1 */

    /* Estimate the row index r such that t_r ≤ n < t_{r+1}. */
    double d = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) * 0.5);

    /* Correct possible rounding errors. */
    while (triangular(r + 1) <= n)
        ++r;
    while (r > 0 && triangular(r) > n)
        --r;

    /* Position inside the row. */
    idx_t k = n - triangular(r);

    if (k == r)        /* last element of row r */
        return pow2((idx_t)(r - 1));
    else                /* 0 ≤ k < r : binomial coefficient */
        return binom_n_k((idx_t)(r - 1), k);
}
