/*
 * A237114  Smallest semiprime of the form k^prime(n)+1, or 0 if no such
 *          semiprime exists.
 * https://oeis.org/A237114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237114:  Smallest semiprime of the form k^prime(n)+1,
 * with both k+1 and (k^prime(n)+1)/(k+1) prime, except at n=1
 * where the restriction on k+1 is dropped.
 *
 * This routine computes the term by brute force search for k,
 * using the helper functions:
 *   get_prime   : retrieve the n-th prime
 *   big_pow     : exact exponentiation
 *   is_prime_big: primality test for arbitrary width integers
 *
 * Complexity is dominated by the big_pow call for the smallest
 * valid k, which for most n is k=2, so per call is very fast.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Handle the exceptional offset n==1 */
    if (n == 1) {
        /* Find the smallest k such that k^2+1 is a semiprime */
        for (uint64_t k = 2; ; ++k) {
            seq_t val = (seq_t)k * (seq_t)k + 1;
            if (is_prime_big(val))
                continue;                       /* skip if prime */
            /* trial division up to sqrt(val) */
            for (seq_t d = 2; d * d <= val; ++d) {
                if (val % d == 0) {
                    seq_t other = val / d;
                    if (is_prime_big(d) && is_prime_big(other))
                        return val;
                }
            }
        }
    }

    /* n >= 2: standard definition */
    seq_t p = (seq_t)get_prime((idx_t)n);       /* p = prime(n)  */
    for (uint64_t k = 2; ; ++k) {
        seq_t k1 = (seq_t)k + 1;                /* k+1       */
        if (!is_prime_big(k1))
            continue;                           /* need k+1 prime */

        seq_t val = big_pow((seq_t)k, p) + 1;   /* k^p + 1   */
        if (val % k1 != 0)
            continue;                           /* division must be exact */

        seq_t q = val / k1;                     /* the other factor */
        if (is_prime_big(q))
            return val;                         /* semiprime found */
    }
}
