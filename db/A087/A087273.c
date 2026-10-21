/*
 * A087273  Largest prime factor of 3*prime(n) + 1.
 * https://oeis.org/A087273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006530(1 + 3*A000040(n)) (largest prime factor of
 *            3*prime(n)+1)
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (index starts at 1) */
    uint32_t p = get_prime(n);

    /* Compute 3*p + 1 using seq_t arithmetic */
    seq_t m = ((seq_t)3 * p) + 1;

    /* Factor m and return its largest prime factor.
       The value of m for the tested n-range fits in 64 bits,
       so we can safely cast to uint64_t for the factoring routine. */
    uint64_t m64 = (uint64_t)m;
    uint64_t lp = largest_prime_factor(m64);

    return (seq_t)lp;
}
