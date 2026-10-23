/*
 * A100217  Diagonal sums of a binomial number triangle.
 * https://oeis.org/A100217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*n-3*k-1, n-2*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    /* Handle the offset 0 separately: a(0) = 1 */
    if (n == 0) return (seq_t)1;

    idx_t max_k = n / 2;
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= max_k; ++k)
    {
        idx_t m =  (idx_t)(2 * n - 3 * k - 1);
        idx_t r =  (idx_t)(n - 2 * k);
        sum += binom_n_k(m, r);
    }

    return sum;
}
