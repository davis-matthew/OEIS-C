/*
 * A063928  Largest nonprime proper divisor of n (with a(1)=1).
 * https://oeis.org/A063928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest nonprime proper divisor of n (with a(1)=1)
 * Range:     u8 n <= 511, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t n) */

/* Largest nonprime proper divisor of n */
seq_t a(idx_t n)
{
    /* a(1) is defined to be 1; for n=1 we return 1 directly. */
    if (n <= 2) {
        /* For n = 1 or 2: proper divisors are {1};
           1 is not prime, so return 1. */
        return 1;
    }

    /* Find the smallest prime factor (spf) of n.  If none are found,
       n is prime. */
    idx_t spf = 0;
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            spf = d;
            break;
        }
    }
    if (spf == 0) {
        /* n is prime, only proper divisor is 1. */
        return 1;
    }

    /* Candidate largest nonprime divisor is n / spf. */
    idx_t q = n / spf;
    if (q > 1 && !is_prime_big((seq_t)q)) {
        /* If q is composite, it is the largest nonprime proper divisor. */
        return (seq_t)q;
    }

    /* If q is prime (or q==1), the only nonprime proper divisor is 1. */
    return 1;
}
