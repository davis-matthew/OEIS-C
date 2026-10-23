/*
 * A195162  Generalized 12-gonal numbers: k*(5*k-4) for k = 0, +-1, +-2, ...
 * https://oeis.org/A195162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {0 if n==0; k*(5*k-4) where k = (n+1)/2 when n is odd, k
 *            = -n/2 when n is even}
 * Range:     u8 n <= 13, u16 n <= 228, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    int64_t k;
    if (n & 1)
        k = (int64_t)(n + 1) / 2;
    else
        k = - (int64_t)(n / 2);

    int64_t term = k * (5 * k - 4);
    return (seq_t)term;
}
