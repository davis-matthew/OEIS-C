/*
 * A076368  a(1) = 1; for n > 1, a(n) = prime(n) - prime(n-1) + 1.
 * https://oeis.org/A076368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - prime(n-1) + 1 (a(1)=1)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided get_prime(idx_t k) utility for prime access. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/get_prime.h"   /* provides: int get_prime(idx_t k); */

/* Return the nth term of A076368.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; unused value for n==0 */
        return (seq_t)0;
    if (n == 1)           /* by definition */
        return (seq_t)1;

    /* get_prime returns the k-th prime as int; prime values are < 2^31 for
       n up to 10 000, so the difference fits in seq_t. */
    int p_n   = get_prime(n);
    int p_n1  = get_prime(n - 1);

    seq_t diff = (seq_t)( (seq_t)p_n - (seq_t)p_n1 + (seq_t)1 );
    return diff;
}
