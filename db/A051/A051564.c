/*
 * A051564  Second unsigned column of triangle A051523.
 * https://oeis.org/A051564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * Σ_{k=0}^{n-1} binom(9+k,k) / (n-k) (a(0)=0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"    /* provides seq_t factorial(idx_t n)    */
#include "../utils/binomial.h"     /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset 0, a(0)=0 */
        return 0;

    seq_t fac = factorial(n);   /* n!  */
    seq_t sum = 0;
    seq_t bin = 1;              /* binom(9+0,0) */

    for (idx_t k = 0; k < n; ++k)
    {
        /* term = fac / (n-k) * binom(9+k,k) */
        seq_t term = (fac / (n - k)) * bin;
        sum += term;

        /* update binom(9+(k+1),k+1) */
        seq_t numerator = bin * (seq_t)(9 + k + 1);
        bin = numerator / (k + 1);   /* exact division */
    }

    return sum;
}
