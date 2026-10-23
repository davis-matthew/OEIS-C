/*
 * A121676  a(n) = [x^n] (1 + x*(1+x)^(n-1) )^(n+1).
 * https://oeis.org/A121676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n+1} C(n+1,k) * C((n-1)*k, n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t c1 = binom(n + 1, k);                  /* C(n+1,k) */
        seq_t c2 = binom((n - 1) * k, n - k);        /* C((n-1)*k , n-k) */
        sum += (c1 * c2);
    }
    return sum;
}
