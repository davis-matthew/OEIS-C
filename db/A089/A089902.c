/*
 * A089902  Antidiagonal sums of array A089900.
 * https://oeis.org/A089902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Sum_{i=0..k} (n-k)^(k-i) * binomial(k, i) *
 *            (i+1)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t k = 0; k <= n; ++k) {
        idx_t r = n - k;          /* r = n - k  (row index of the array) */
        seq_t inner = 0;          /* inner sum for this k */

        if (r == 0) {            /* when r = 0 only the term i = k survives */
            inner = factorial(k + 1);   /* (k+1)!  */
        } else {
            seq_t term = big_pow((seq_t)r, (seq_t)k);  /* r^k  */
            inner = term;

            for (idx_t i = 1; i <= k; ++i) {
                /* term_{i} = term_{i-1} * (k-i+1)*(i+1) / (i*r)   */
                term = term * (seq_t)(k - i + 1);
                term = term * (seq_t)(i + 1);
                term = term / (seq_t)(i * r);
                inner += term;
            }
        }

        result += inner;
    }

    return result;
}
