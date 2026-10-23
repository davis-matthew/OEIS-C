/*
 * A008866  Prime(A052928(n+1)) + (-1)^n* prime(A109613(n)).
 * https://oeis.org/A008866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+1) + (-1)^n * prime(n)
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A008866 using the provided
 * get_prime utility (returns the k-th prime).  The sequence
 * is defined for n >= 1; for n == 0 the function returns 0
 * as a dummy value. */

#include "../utils/get_prime.h"   /* get_prime(idx_t k) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset guard */

    /* Get the nth and (n+1)th primes.  The returned value is
     * an unsigned 32‑bit integer, which we safely cast to seq_t. */
    seq_t p_n   = (seq_t)get_prime(n);     /* prime(n)   */
    seq_t p_np1 = (seq_t)get_prime(n + 1); /* prime(n+1) */

    /* For odd n, subtract; for even n, add. */
    if (n & 1)
        return p_np1 - p_n;
    else
        return p_np1 + p_n;
}
