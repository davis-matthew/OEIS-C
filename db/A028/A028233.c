/*
 * A028233  If n = p_1^e_1 * ... * p_k^e_k, p_1 < ... < p_k primes, then a(n)
 *          = p_1^e_1, with a(1) = 1.
 * https://oeis.org/A028233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the highest power of the smallest prime dividing n.
   For n = p1^e1 * p2^e2 * ..., a(n) = p1^e1.  The case a(1) = 1 is
   handled explicitly.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;            /* a(1) = 1, a(0) not used */

    /* Find the smallest prime factor spf of n. */
    idx_t spf = n;                  /* default if n is prime */
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            spf = d;
            break;
        }
    }

    /* Compute the largest power of spf dividing n. */
    idx_t temp = n;
    seq_t result = 1;
    while (temp % spf == 0) {
        result *= (seq_t)spf;
        temp /= spf;
    }
    return result;
}
