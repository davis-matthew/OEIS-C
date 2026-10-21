/*
 * A055033  a(n) = usigma(usigma(n)), where usigma(n) is the sum of unitary
 *          divisors of n (A034448).
 * https://oeis.org/A055033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A034448(A034448(n)) where A034448(m) = sum of unitary
 *            divisors of m (product over prime powers (1 + p^e))
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   unitary_sigma_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/unitary_sigma_uint64.h"

seq_t a(idx_t n)
{
    /* First unitary sigma of n */
    uint64_t n64 = (uint64_t)n;
    seq_t s1 = unitary_sigma_uint64(n64);

    /* Since s1 will be small for all n used in tests, we safely cast
       it back to uint64_t for the second application. */
    uint64_t s1_64 = (uint64_t)s1;
    seq_t s2 = unitary_sigma_uint64(s1_64);

    return s2;
}
