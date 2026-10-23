/*
 * A193295  Number of prime divisors (with multiplicity) of n^2 - 1.
 * https://oeis.org/A193295
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(n^2 - 1) (number of prime divisors with
 *            multiplicity of n²−1)
 * Range:     u8 n <= 16, u16 n <= 667, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 2; behaviour does not matter. */
    if (n < 2)
        return 0;
    /* Compute n² – 1 without overflow for the supported seq_t sizes. */
    seq_t m = (seq_t)n * (seq_t)n - 1;
    return bigomega((idx_t)m);
}
