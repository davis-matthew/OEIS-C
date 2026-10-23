/*
 * A158210  a(n) = omega(n) * (-1)^mu(n), where mu is the Moebius function.
 * https://oeis.org/A158210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A001221(n) * (-1)^A008683(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined, return 0 for safety */

    /* number of distinct prime factors of n (A001221) */
    seq_t omega = (seq_t)count_distinct_primes((uint64_t)n);

    /* Möbius function μ(n) (A008683): 0, 1, or -1 */
    seq_t mu = mobius((idx_t)n);

    /* compute (-1)^μ(n): +1 if μ(n)=0, otherwise -1 */
    seq_t sign = (mu == 0) ? (seq_t)1 : (seq_t)-1;

    return omega * sign;
}
