/*
 * A023519  Least odd prime divisor of prime(n)*prime(n-1) - 1, or 1 if
 *          prime(n)*prime(n-1) - 1 is a power of 2.
 * https://oeis.org/A023519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   get_prime, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A023519: a(n) = least odd prime divisor of prime(n)*prime(n-1) - 1,
   or 1 if prime(n)*prime(n-1)-1 is a power of 2.
   OEIS offset: 1 (a(1) is the first term listed)
*/

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    /* Ensure offset 1: for n==0, return 1 (value undefined) */
    if (n == 0) {
        return 1;
    }

    /* Retrieve primes using 1‑based convention (prime(1)=2). */
    uint64_t p_n   = get_prime(n);                     /* prime(n) */
    uint64_t p_prev = (n == 1) ? 1ULL : get_prime(n-1); /* prime(n-1), with prime(0)=1 */

    /* Compute the product minus one. */
    seq_t val = (seq_t)p_n * (seq_t)p_prev - 1;

    /* If the value is a power of 2, the least odd prime divisor is defined as 1. */
    if (is_power_of_two(val)) {
        return 1;
    }

    /* Remove all factors of 2 to obtain the odd part. */
    seq_t odd_part = val;
    while ((odd_part % 2) == 0) {
        odd_part /= 2;
    }

    /* Search for the smallest odd prime divisor of odd_part. */
    for (idx_t k = 2; ; ++k) {
        uint64_t q = get_prime(k);           /* candidate odd prime */
        seq_t qseq = (seq_t)q;
        seq_t sq = qseq * qseq;              /* q^2 */
        if (sq > odd_part) {
            break;                          /* all remaining primes are > sqrt(odd_part) */
        }
        if ((odd_part % qseq) == 0) {
            return (seq_t)q;                 /* found the least odd prime divisor */
        }
    }

    /* If no proper divisor found, odd_part itself is the least odd prime divisor. */
    return odd_part;
}
