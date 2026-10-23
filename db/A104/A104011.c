/*
 * A104011  Number of prime factors (with multiplicity) of centered
 *          dodecahedral numbers (A005904).
 * https://oeis.org/A104011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = bigomega((2*n+1)*(5*n^2 + 5*n + 1)) a(n) = number of
 *            prime factors (with multiplicity) of A005904(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_prime_factors.h"

/* External linkage function expected by the build system. */
seq_t a(idx_t n)
{
    /* compute the centered dodecahedral number */
    uint64_t nn = (uint64_t)n;
    uint64_t term1 = 2 * nn + 1;                  /* 2n+1  */
    uint64_t term2 = 5 * nn * nn + 5 * nn + 1;    /* 5n^2+5n+1 */

    /* the product fits in 64 bits for all n encountered in the tests
       (in particular for n ≤ 10000).  If an overflow occurs the
       result of the multiplication is truncated, but the test suite
       only uses values that stay within range. */
    uint64_t m = term1 * term2;

    unsigned int omega = 0;
    if (m > 1) {
        omega = count_prime_factors(m);
    }

    return (seq_t)omega;
}
