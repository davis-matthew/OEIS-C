/*
 * A056898  a(n) = smallest number m such that m^2+n is prime.
 * https://oeis.org/A056898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 365, u16 n <= 9805, u32 n <= 9805, u64 n <= 9805,
 *            u128 n <= 9805, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest m such that m^2 + n is prime.
 * Offset 1: a(1)=1, a(2)=0, a(3)=0, a(4)=1, ...
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t sn = (seq_t)n;

    /* Search m starting from 0 */
    for (seq_t m = (seq_t)0; ; ++m)
    {
        /* Compute m^2 + n using seq_t arithmetic */
        seq_t val = m * m + sn;

        /* Check whether val is prime */
        if (is_prime_big(val))
            return m;
    }
}
