/*
 * A050152  a(n) = T(n,n+3), array T as in A050143.
 * https://oeis.org/A050152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binom(n,k) * binom(2*n - k + 1, n - k - 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A050152 – T(n,n+3) for the array in A050143) */

#include <stdint.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t c1 = binom(n, k);                        /* binom(n,k) */
        seq_t c2 = binom(2 * n - k + 1, n - k - 1);    /* binom(2n-k+1, n-k-1) */
        sum += c1 * c2;
    }
    return sum;
}
