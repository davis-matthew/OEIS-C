/*
 * A105574  a(1) = 2; for n > 1, a(n) is the prime whose index is the least
 *          prime factor of n.
 * https://oeis.org/A105574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = the n-th prime, where n is the smallest prime factor of
 *            the argument (with a(1) = 2)
 * Range:     u8 n <= 58, u16 n <= 6546, u32 n <= 9838, u64 n <= 9838,
 *            u128 n <= 9838, big any n
 * Helpers:   get_prime, integer_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"          /* seq_t get_prime(uint64_t idx); */
#include "../utils/integer_sqrt.h"      /* idx_t integer_sqrt(idx_t x); */

/* Compute the smallest prime factor of n (n >= 2). */
static inline idx_t smallest_prime_factor(idx_t n)
{
    if ((n & 1U) == 0) return 2;               /* even number */
    idx_t limit = integer_sqrt(n);
    for (idx_t i = 3; i <= limit; i += 2) {
        if (n % i == 0) return i;
    }
    return n;        /* n is prime */
}

/* OEIS sequence A105574 */
seq_t a(idx_t n)
{
    if (n == 1) return 2;                     /* by definition */
    idx_t spf = smallest_prime_factor(n);     /* smallest prime factor of n */
    return get_prime(spf);                    /* prime with index = spf */
}
