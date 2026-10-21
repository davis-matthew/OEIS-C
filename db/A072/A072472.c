/*
 * A072472  a(n) = product of numbers from prime(n)+1 up to prime(n+1), where
 *          prime(n) is the n-th prime.
 * https://oeis.org/A072472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=prime(n)+1}^{prime(n+1)} k (product of all integers
 *            strictly between the n-th and the (n+1)-st prime, inclusive of
 *            the latter end point)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 98,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"   /* provides: uint32_t get_prime(idx_t k); */

seq_t a(idx_t n)
{
    /* The sequence has offset 1; values for n < 1 are undefined but we
       return 0 to keep the function safe. */
    if (n == 0) return 0;

    /* Get the bounding primes */
    idx_t p1 = get_prime(n);       /* n-th prime */
    idx_t p2 = get_prime(n + 1);   /* (n+1)-st prime */

    seq_t result = 1;              /* the multiplicative identity in seq_t */

    /* Multiply all integers from p1+1 up to p2 inclusive. */
    for (idx_t v = p1 + 1; v <= p2; ++v)
        result *= (seq_t)v;

    return result;
}
