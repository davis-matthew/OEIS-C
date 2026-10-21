/*
 * A080086  Number of factors of 3 in the factorial of the n-th prime, counted
 *          with multiplicity.
 * https://oeis.org/A080086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=1} floor( p_n / 3^k ) where p_n is the nth prime
 *            (n>=1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes the required helper to obtain the nth prime. */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Retrieve the nth prime. get_prime returns uint32_t, enough for n up to the limits considered. */
    uint32_t prime = get_prime(n);

    seq_t result = 0;
    uint64_t divisor = 3ULL;

    /* Accumulate the Legendre exponent for base 3. */
    while (divisor <= (uint64_t)prime) {
        result += (seq_t)(prime / divisor);
        if (divisor > ((uint64_t)prime) / 3ULL) {
            break;   /* avoid overflow of divisor *= 3 */
        }
        divisor *= 3ULL;
    }

    return result;
}
