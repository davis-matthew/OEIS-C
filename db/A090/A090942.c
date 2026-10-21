/*
 * A090942  n-th arithmetic mean = prime(n).
 * https://oeis.org/A090942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*prime(n) - (n-1)*prime(n-1) (OEIS A090942)
 * Range:     u8 n <= 24, u16 n <= 1532, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility function get_prime(idx_t k) from ../utils/get_prime.h */

#include "../utils/get_prime.h"

/* Compute the n-th term of OEIS A090942 */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For safety, return 0 for n == 0. */
    if (n == 0)
        return (seq_t)0;

    /* First term: a(1) = prime(1) */
    if (n == 1)
        return (seq_t)get_prime(1);

    /* General case */
    seq_t pn   = (seq_t)get_prime(n);      /* prime(n)  */
    seq_t pn_1 = (seq_t)get_prime(n-1);    /* prime(n-1) */

    /* Compute n * prime(n) - (n-1) * prime(n-1) */
    return (seq_t)n * pn - (seq_t)(n-1) * pn_1;
}
