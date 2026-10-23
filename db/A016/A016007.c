/*
 * A016007  a(n) = (tau(n^11)+10)/11.
 * https://oeis.org/A016007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (tau(n^11)+10)/11
 * Range:     u8 n <= 11, u16 n <= 3299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The divisor count of n^11 is ∏ (11*e_i + 1), where e_i are
       the exponents in the prime factorisation of n. */
    seq_t prod = (seq_t)1;     /* product of the (11*e_i+1) factors */
    uint64_t m = (uint64_t)n;

    /* Handle the trivial case n == 1 early: τ(1)=1. */
    if (m <= 1) {
        return (seq_t)1;  /* (1 + 10)/11 = 1 */
    }

    /* Factor out the power of 2 first. */
    uint64_t exp = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++exp;
    }
    if (exp > 0) {
        prod = prod * ((seq_t)11 * (seq_t)exp + (seq_t)1);
    }

    /* Factor odd primes up to sqrt(m). */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            prod = prod * ((seq_t)11 * (seq_t)exp + (seq_t)1);
        }
    }

    /* If m > 1, then it is a prime factor with exponent 1. */
    if (m > 1) {
        prod = prod * ((seq_t)11 * (seq_t)1 + (seq_t)1);  /* 11*1+1 = 12 */
    }

    /* Compute (τ(n^11)+10)/11. */
    seq_t result = (prod + (seq_t)10) / (seq_t)11;
    return result;
}
