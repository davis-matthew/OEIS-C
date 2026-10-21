/*
 * A227733  a(n) = sigma(2*n)^2 - sigma(n)^2.
 * https://oeis.org/A227733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(2*n)^2 - sigma(n)^2
 * Range:     u8 n <= 4, u16 n <= 59, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_single
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sigma_single.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t s1 = sigma_single(2 * n);
    seq_t s2 = sigma_single(n);
    return s1 * s1 - s2 * s2;
}
