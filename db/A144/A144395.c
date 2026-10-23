/*
 * A144395  A designed polynomial set that gives a {1,6,1} quadratic and gives
 *          a symmetrical triangle of coefficients: p(x,n)=If[n == 2, 1, ((x +
 *          1)^n -If[n == 0, 1, x^n + (n - 1)*x^(n - 1) + (n - 1)*x + 1])/x],.
 * https://oeis.org/A144395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   Let k be the smallest integer with k(k+1)/2 ≥ n. Let p = n –
 *            k(k‑1)/2 – 1 (0‑based position in the k‑th row). Then a(n) = 1
 *            if p == 0 or p == k‑1, a(n) = binomial(k+1, p+1) otherwise.
 *            (Here binomial(i,j) denotes the standard binomial coefficient.)
 * Range:     u8 n <= 48, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binomial_pascal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial_pascal.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for offset 1, but safety */

    /* find the row k such that k(k+1)/2 >= n */
    idx_t k = 1;
    while (((k * (k + 1)) / 2) < n) ++k;

    idx_t prev_sum = ((k - 1) * k) / 2;
    idx_t pos = n - prev_sum;          /* 1‑based position in the row */
    idx_t e   = pos - 1;               /* 0‑based exponent inside the row */

    if (e == 0 || e == k - 1) return (seq_t)1;

    idx_t N = k + 1;                   /* polynomial degree + 1 */
    idx_t r = e + 1;                   /* binomial column index */

    return binomial_pascal(N, r);
}
