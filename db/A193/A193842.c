/*
 * A193842  Triangular array: the fission of the polynomial sequence ((x+1)^n:
 *          n >= 0) by the polynomial sequence ((x+2)^n: n >= 0). (Fission is
 *          defined at Comments.)
 * https://oeis.org/A193842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = T(r,k) where r and k satisfy r(r+1)/2 <= n <
 *            (r+1)(r+2)/2, T(r,k) = Sum_{i=0..k} 3^(k-i) * binom(r-i, k-i)
 *            (OEIS A193842)
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 165, u64 n <= 616,
 *            u128 n <= 2255, big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"   /* provides static seq_t binom(idx_t a, idx_t b)   */
#include "../utils/pow3.h"    /* provides seq_t pow3(idx_t exp)              */

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2.  */
    idx_t r = 0;
    while ( (r + 1) * (r + 2) / 2 <= n )
        ++r;

    /* Column k within that row.  */
    idx_t k = n - r * (r + 1) / 2;

    /* Sum the defining expression. */
    seq_t sum = 0;
    for (idx_t i = 0; i <= k; ++i)
    {
        idx_t exp = k - i;                 /* exponent for 3 */
        seq_t term = binom(r - i, exp);    /* binomial coefficient C(r-i, exp) */
        term = term * pow3(exp);           /* multiply by 3^exp          */
        sum = sum + term;
    }
    return sum;
}
