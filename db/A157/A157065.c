/*
 * A157065  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 32.
 * https://oeis.org/A157065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n,16); T(n,k) = Sum_{i=1..n}
 *            binomial(n+1,i)*binomial(k-1,i-1)*binomial(n-i+k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 24, u128 n <= 107,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t i;
    seq_t total = 0;

    for (i = 1; i <= n; ++i)
    {
        seq_t c1 = binomial(n + 1, i);            /* binomial(n+1,i) */
        seq_t c2 = binomial(15, i - 1);           /* binomial(16-1,i-1) */
        seq_t c3 = binomial(n - i + 16, 16);     /* binomial(n-i+16,16) */

        seq_t term = c1 * c2;
        term *= c3;
        total += term;
    }

    return total;
}
