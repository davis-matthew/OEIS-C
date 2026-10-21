/*
 * A226982  a(n) = ceiling(n/2) - primepi(n).
 * https://oeis.org/A226982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n/2) - primepi(n) = floor((n+1)/2) - pi(n)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility function prime_pi from ../utils/prime_pi.h */
#include <stdint.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* Offset is 1, but the function is safe for n >= 1 */
    if (n == 0)
        return 0;   /* The sequence is undefined for n < 1, return 0 as a guard */

    seq_t half = ((seq_t)n + 1) / 2;   /* ceil(n/2) */
    seq_t pi_n  = prime_pi(n);        /* number of primes <= n */
    return half - pi_n;
}
