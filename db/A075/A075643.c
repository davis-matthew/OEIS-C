/*
 * A075643  Group the natural numbers so that the n-th group contains n
 *          numbers one each of a multiple of numbers from 1 to n so that the
 *          group sum is a multiple of (n+1): (2), (1, 8), (3, 4, 9), (5, 6,
 *          12, 32), (7, 10, 15, 16, 30), (11, 14, 18, 20, 25, 24), ...
 *          Sequence gives initial terms of groups.
 * https://oeis.org/A075643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(2) = 1; for n > 2, a(n) = prime(n-1)
 * Range:     u8 n <= 55, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;
    return (seq_t)get_prime(n - 1);
}
