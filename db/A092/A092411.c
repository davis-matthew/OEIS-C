/*
 * A092411  a(n) = sigma(n,2) + sigma(n+1,2).
 * https://oeis.org/A092411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n,2) + sigma(n+1,2) where sigma(m,2) = Σ_{d|m} d^2.
 *            The implementation uses a simple O(√m) divisor enumeration
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   sigma2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/sigma2.h"

seq_t a(idx_t n)
{
    /* n >= 1 guaranteed by OEIS offset */
    seq_t s1 = sigma2(n);
    seq_t s2 = sigma2(n + (idx_t)1);
    return s1 + s2;
}
