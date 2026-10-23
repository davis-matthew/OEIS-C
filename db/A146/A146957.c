/*
 * A146957  A functionally symmetric Polynomial as a triangle of coefficients:
 *          p(x,n)=If[n == 0, 1, (x + 1)^n + 2^(n - 2)*Sum[(3^(m-1) + 2*m-1
 *          )*x^m*(1 + x^(n - 2*m)), {m, 1, n - 1}]].
 * https://oeis.org/A146957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A146957 – flattened triangle coefficients of
   p(x,n) = (x + 1)^n
        + 2^(n-2) * Σ_{m=1}^{n-1} (3^(m-1) + 2m - 1) * x^m * (1 + x^(n-2m))  */
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Convert the linear index n into row r and column k */
/*    row r satisfies  r(r+1)/2  <=  n  <  (r+1)(r+2)/2  */
    idx_t r = 0;
    while (((r + 1U) * (r + 2U)) / 2U <= n) {
        ++r;
    }
    idx_t k = n - (r * (r + 1U)) / 2U;          /* 0 ≤ k ≤ r */

    /* Base coefficient from (x+1)^r  */
    seq_t coeff = binomial(r, k);

    if (r >= 2U) {
        seq_t two_pow = pow2(r - 2U);           /* 2^(r-2) */

        /* Contribution from m = k  */
        if (k >= 1U && k <= r - 1U) {
            seq_t ck = pow3(k - 1U) + (seq_t)(2U * k) - (seq_t)1;
            coeff += two_pow * ck;
        }

        /* Contribution from m = r-k  */
        idx_t m = r - k;
        if (m >= 1U && m <= r - 1U) {
            seq_t cm = pow3(m - 1U) + (seq_t)(2U * m) - (seq_t)1;
            coeff += two_pow * cm;
        }
    }

    return coeff;
}
