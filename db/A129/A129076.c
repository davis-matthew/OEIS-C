/*
 * A129076  a(n) = sigma(sigma(sigma(sigma(n)))), where sigma(n) = sum of
 *          divisors of n.
 * https://oeis.org/A129076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(sigma(sigma(sigma(n))))
 * Range:     u8 n <= 11, u16 n <= 659, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(n) = sum of all positive divisors of n. */

#include <stdint.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* First sigma */
    seq_t x1 = sigma_of(n);
    /* Convert to idx_t for the next call */
    idx_t n1 = (idx_t)x1;

    /* Second sigma */
    seq_t x2 = sigma_of(n1);
    idx_t n2 = (idx_t)x2;

    /* Third sigma */
    seq_t x3 = sigma_of(n2);
    idx_t n3 = (idx_t)x3;

    /* Fourth sigma */
    seq_t x4 = sigma_of(n3);

    return x4;
}
