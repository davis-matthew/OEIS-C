/*
 * A023522  Sum of exponents in prime-power factorization of p(n)*p(n-1) - 1.
 * https://oeis.org/A023522
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(p(n)*p(n-1)-1)
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of exponents in the prime-power factorization of p(n)*p(n-1)-1.
   p(k) is the k‑th prime.
   Uses get_prime(k) to obtain primes and bigomega(x) to sum exponents. */

#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/bigomega.h"   /* seq_t bigomega(idx_t n); */

seq_t a(idx_t n)
{
    if (n < 2)            /* undefined for n < 2; return 0 */
        return 0;

    /* obtain the n-th and (n-1)-th prime */
    uint64_t pn  = (uint64_t)get_prime(n);
    uint64_t pn1 = (uint64_t)get_prime(n - 1);

    /* compute p(n)*p(n-1)-1 in seq_t to avoid overflow if seq_t > 64 bits */
    seq_t val = (seq_t)pn * (seq_t)pn1 - 1;

    /* bigomega expects an idx_t argument; cast val safely (fits in 64 bits) */
    return bigomega((idx_t)val);
}
