/*
 * A163637  The sum of all odd numbers from 2*n-1 to prime(n).
 * https://oeis.org/A163637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of all odd numbers from 2*n-1 up to the n-th prime
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The lower bound is always odd. */
    seq_t lower = (seq_t)2 * n - (seq_t)1;

    /* Get the n-th prime. */
    seq_t p = (seq_t)get_prime(n);

    /* Sum of odd numbers up to p is ((p+1)/2)^2. */
    seq_t m1 = (p + (seq_t)1) / (seq_t)2;
    seq_t sum_upper = m1 * m1;

    /* Sum of odd numbers up to the previous odd before 'lower';
       that odd is lower-2, so it is ((lower-1)/2)^2. */
    seq_t m2 = (lower - (seq_t)1) / (seq_t)2;
    seq_t sum_lower_prev = m2 * m2;

    return sum_upper - sum_lower_prev;
}
