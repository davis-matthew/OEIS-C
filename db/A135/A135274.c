/*
 * A135274  a(n) = prime(2*n) - prime(2*n-1) + prime(2*n+1).
 * https://oeis.org/A135274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(2*n) - prime(2*n-1) + prime(2*n+1)
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n must be >= 1 */
    seq_t p2n  = (seq_t)get_prime(2*n);   /* prime(2*n)   */
    seq_t p2n_1= (seq_t)get_prime(2*n-1); /* prime(2*n-1) */
    seq_t p2n1 = (seq_t)get_prime(2*n+1); /* prime(2*n+1) */
    return p2n - p2n_1 + p2n1;
}
