/*
 * A093545  Sorted mapping of A093544 onto the integers.
 * https://oeis.org/A093545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(3n) = 5n, * a(3n+1) = 5n + 2, * a(3n+2) = A047206(n), * where
 *            A047206(n) is the increasing sequence of integers congruent to *
 *            1, 3, or 4 modulo 5 (i.e. the sequence 1,3,4,6,8,9,11,13,14,…)
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A047206
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/A047206.h"

/* Main sequence A093545. */
seq_t
a(idx_t n)
{
    idx_t q = n / 3;
    switch (n % 3) {
    case 0:                             /* a(3n)   = 5n */
        return (seq_t)q * ((seq_t)5);
    case 1:                             /* a(3n+1) = 5n + 2 */
        return (seq_t)q * ((seq_t)5) + (seq_t)2;
    default:                            /* a(3n+2) = A047206(n/3) */
        return A047206(q);
    }
}
