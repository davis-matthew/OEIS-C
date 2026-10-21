/*
 * A235799  a(n) = n^2 - sigma(n).
 * https://oeis.org/A235799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - sigma(n)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; behaviour for n<1 is irrelevant */
        return 0;
    seq_t N     = (seq_t)n;
    seq_t sigma = sigma_of(n);
    return N * N - sigma;
}
