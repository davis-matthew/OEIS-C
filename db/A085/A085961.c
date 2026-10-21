/*
 * A085961  Sigma of the least number for which sigma is divisible by n (where
 *          sigma is the sum of the divisors of n, A000203(n)).
 * https://oeis.org/A085961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000203(A070982(n)) (the sum of divisors of the smallest
 *            integer m whose divisor sum is divisible by n)
 * Range:     u8 n <= 28, u16 n <= 1666, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /*  n is at least 1 (sequence offset is 1) */
    idx_t m = 1;                /* candidate integer */
    seq_t divsum;

    while (1) {
        divsum = sigma_of(m);          /* σ(m) */
        if ((divsum % (seq_t)n) == 0)  /* σ(m) divisible by n? */
            return divsum;             /* σ(m) is the desired value */
        ++m;                           /* try the next integer */
    }
}
