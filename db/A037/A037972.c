/*
 * A037972  a(n) = n^2*(n+1)*binomial(2*n-2, n-1)/2.
 * https://oeis.org/A037972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*(n+1)*binomial(2*n-2, n-1)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t sn = (seq_t)n;          /* n as seq_t */
    seq_t bin = binomial(2*n - 2, n - 1);  /* binomial(2n-2, n-1) */
    seq_t res = sn * sn;          /* n^2 */
    res *= (seq_t)(n + 1);        /* n^2*(n+1) */
    res *= bin;                  /* *binomial */
    res /= (seq_t)2;              /* divide by 2 */
    return res;
}
