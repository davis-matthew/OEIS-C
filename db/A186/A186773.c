/*
 * A186773  Odd numbers whose decimal digits sum to a prime.
 * https://oeis.org/A186773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th odd integer whose decimal digit sum is a prime,
 *            counted from n = 1
 * Range:     u8 n <= 48, u16 n <= 9032, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"       /* seq_t digit_sum(seq_t v); */
#include "../utils/is_prime_small.h"  /* bool is_prime_small(seq_t n); */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so a(1) is the first term. */
    if (n == 0) return (seq_t)0;           /* undefined for zero index */

    seq_t candidate = (seq_t)1;             /* start before the first odd */ 
    idx_t found = 0;                        /* number of valid terms found */

    while (found < n) {
        candidate += (seq_t)2;              /* next odd integer */

        /* Check if its digit sum is prime. */
        if (is_prime_small(digit_sum(candidate))) {
            ++found;                        /* register as the next term */
        }
    }

    return candidate;                       /* candidate is the n-th term */
}
