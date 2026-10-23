/*
 * A117375  Riordan array (1/(1-3x*c(x)),xc(x)), c(x) the g.f. of A000108.
 * https://oeis.org/A117375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle obtained from the
 *            Riordan array product (1, x*c(x))*(1/(1-3x), x), where c(x) is
 *            the generating function of the Catalan numbers. Elements are
 *            read in row-major order: a(0) = T(0,0), a(1) = T(1,0), a(2) =
 *            T(1,1), a(3) = T(2,0),
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binom.h"   /* binom_n_k : seq_t binom(idx_t n, idx_t k)  */
#include "../utils/pow3.h"    /* pow3 : seq_t pow3(idx_t e)                 */

seq_t a(idx_t n)
{
    /* special case n = 0 */
    if (n == 0) return 1;

    /* compute the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t offset = r * (r + 1) / 2;          /* linear index of T(r,0)   */
    idx_t k = n - offset;                    /* column index              */

    seq_t sum = 0;
    for (idx_t j = k; j <= r; ++j) {
        idx_t m = r - j;                     /* exponent in c(x)^j        */
        seq_t coeff;

        if (j == 0) {                         /* coefficient of c(x)^0      */
            coeff = (m == 0) ? (seq_t)1 : (seq_t)0;
        } else {
            /* coefficient of x^m in c(x)^j
               = j/(2m+j) * binom(2m+j, m)        */
            idx_t N = 2 * m + j;
            seq_t bin = binom(N, m);           /* binom(2m+j, m)             */
            seq_t num  = bin * (seq_t)j;       /* bin * j                    */
            coeff = num / (seq_t)N;           /* exact division              */
        }

        seq_t pow3_val = pow3(j - k);
        sum += pow3_val * coeff;
    }
    return sum;
}
