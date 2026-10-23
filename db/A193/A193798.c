/*
 * A193798  Triangular array: the fusion of polynomial sequences P and Q given
 *          by p(n,x)=(3x+2)^n and q(n,x)=1+x^n.
 * https://oeis.org/A193798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of (3x+2)^{r-1} at x^k (for k ≤ r-1), * or
 *            a(n) = 5^{r-1} (for k = r), * where r = floor((sqrt(8n+1)-1)/2)
 *            and k = n - r(r+1)/2
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binom, pow2, pow3, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes required helper functions. */
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/power5.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Special case for the first term. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute r = floor((sqrt(8n+1)-1)/2). */
    idx_t r = (_floor_isqrt_u64(8 * n + 1) - 1) >> 1;

    /* k = offset within the r‑th row. */
    idx_t T_r = r * (r + 1) / 2;
    idx_t k   = n - T_r;

    /* m = r-1 is the exponent of the polynomial (3x+2). */
    idx_t m = r - 1;

    if (k <= m) {
        /* Coefficient of (3x+2)^m at x^k:
         * C(m,k) * 2^{m-k} * 3^k
         */
        seq_t bin   = binom(m, k);
        seq_t two   = pow2(m - k);
        seq_t three = pow3(k);

        seq_t res = bin * two;
        res *= three;
        return res;
    } else {
        /* The final element of the row: 5^m. */
        return power5(m);
    }
}
