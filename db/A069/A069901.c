/*
 * A069901  Smallest prime factor of n-th triangular number.
 * https://oeis.org/A069901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime factor of n*(n+1)/2, with a(1)=1
 * Range:     u8 n <= 24, u16 n <= 537, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   smallest_prime_factor_of_tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/smallest_prime_factor_of_tri.h"

seq_t a(idx_t n)
{
    return smallest_prime_factor_of_tri(n);
}
