/*
 * A161659  a(n) = the smallest multiple of {the sum of the
 *          prime-factorization exponents of n} that is >= n.
 * https://oeis.org/A161659
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n / bigomega(n)) * bigomega(n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 2)   /* sequence defined for n ≥ 2 */
        return 0;

    seq_t omega = bigomega(n);      /* sum of exponents in prime factorization of n */
    seq_t k = (seq_t)n / omega;     /* floor division */
    if ((seq_t)n % omega != 0)      /* add 1 if remainder exists */
        k += 1;
    return k * omega;
}
