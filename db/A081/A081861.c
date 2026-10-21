/*
 * A081861  a(n) = (1/24)*(sigma_3(2*n-1) - sigma_1(2*n-1)).
 * https://oeis.org/A081861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (sigma_3(2*n-1) - sigma_1(2*n-1)) / 24
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 836, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of k-th powers of divisors in a multiplicative way. */
#include <stdint.h>
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. */
    idx_t m = 2 * n - 1;            /* (2*n-1) is always odd and >= 1 */

    seq_t sigma1 = sigma_k(m, 1);   /* sum of divisors */
    seq_t sigma3 = sigma_k(m, 3);   /* sum of cubes of divisors */

    seq_t diff = sigma3 - sigma1;
    seq_t result = diff / (seq_t)24;
    return result;
}
