/*
 * A115061  a(n) is the number of occurrences of the n-th prime number in
 *          A051697.
 * https://oeis.org/A115061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n+1) - prime(n-1))/2 for n>=3
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Parameters: n is 1-indexed; returns a(1)=3, a(2)=2 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;          /* values below the offset are irrelevant */
    }
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)2;

    /* n >= 3: use the OEIS formula */
    uint32_t p_minus = get_prime(n - 1);   /* prime(n-1) */
    uint32_t p_plus  = get_prime(n + 1);   /* prime(n+1) */

    return ((seq_t)p_plus - (seq_t)p_minus) / 2;
}
