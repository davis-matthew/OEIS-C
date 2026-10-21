/*
 * A066293  a(n) = A000203(n)^2 - A001157(n) = sigma(n)^2 - sigma_2(n).
 * https://oeis.org/A066293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n)^2 - sigma_2(n) (A066293)
 * Range:     u8 n <= 11, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma1_sigma2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */
#include "../utils/sigma1_sigma2.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* seq_t may be unsigned; values for n<1 are irrelevant */

    unsigned __int128 s1, s2;
    sigma1_sigma2((uint64_t)n, &s1, &s2);

    unsigned __int128 result = (s1 * s1) - s2;
    return (seq_t)result;
}
