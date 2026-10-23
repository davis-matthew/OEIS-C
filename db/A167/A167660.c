/*
 * A167660  Chocolate dove bar numerator: a(n) = (Sum_{k=0..floor(n/2)}
 *          k*binomial(n+k,k)*binomial(n,n-2*k)) + (Sum_{k=0..ceiling(n/2)}
 *          k*binomial(n+k-1,k-1)*binomial(n,n-2*k+1)).
 * https://oeis.org/A167660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} k*binomial(n+k,k)*binomial(n,n-2*k)
 *            + Sum_{k=0..ceil(n/2)} k*binomial(n+k-1,k-1)*binomial(n,n-2*k+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* first sum */
    idx_t max1 = n / 2;          /* floor(n/2) */
    for (idx_t k = 1; k <= max1; ++k) {
        seq_t term = (seq_t)k;
        term *= binom(n + k, k);
        term *= binom(n, n - 2 * k);
        sum += term;
    }

    /* second sum */
    idx_t max2 = (n + 1) / 2;    /* ceil(n/2) */
    for (idx_t k = 1; k <= max2; ++k) {
        seq_t term = (seq_t)k;
        term *= binom(n + k - 1, k - 1);
        term *= binom(n, n - 2 * k + 1);
        sum += term;
    }

    return sum;
}
