/*
 * A120257  Triangle of Hankel transforms of certain binomial sums.
 * https://oeis.org/A120257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = T(r,k) where n = r*(r+1)/2 + k (0 ≤ k ≤ r) and T(r,k) =
 *            (cos(πk/2)-sin(πk/2)) * ∏_{j=0}^{r-k-1} C(2k+2+j, k+1) /
 *            C(k+1+j, j)
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 48, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }

    /* Column k within that row */
    idx_t k = n - (r * (r + 1)) / 2;

    /* Compute the product as a reduced fraction prod_num / prod_den */
    seq_t prod_num = (seq_t)1;
    seq_t prod_den = (seq_t)1;

    idx_t m = r - k;                     /* number of factors (may be 0) */
    for (idx_t j = 0; j < m; ++j) {
        seq_t num = binom(2 * k + 2 + j, k + 1);
        seq_t den = binom(k + 1 + j, j);

        /* Multiply and reduce by gcd to keep numbers small */
        prod_num = prod_num * num;
        prod_den = prod_den * den;

        seq_t g = gcd(prod_num, prod_den);
        if (g != (seq_t)0) {
            prod_num /= g;
            prod_den /= g;
        }
    }

    /* The fraction should be integral now */
    seq_t value = prod_num / prod_den;      /* exact division */

    /* Apply sign: + for k mod 4 = 0 or 3, - otherwise */
    if ((k & 3) == 1 || (k & 3) == 2) {
        value = -value;
    }

    return value;
}
