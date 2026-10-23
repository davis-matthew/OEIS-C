/*
 * A030688  Smallest nontrivial extension of n-th square which is a square not
 *          ending 00.
 * https://oeis.org/A030688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest square m² > n² whose decimal representation
 *            starts with the digits of n² and does not end in '00'. This is
 *            the definition of OEIS A030688. The algorithm tests successive
 *            extensions of the prefix n² by multiplying it by powers of 10
 *            and searching the resultant interval for a sq...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 81, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1 */
    if (n < 1) return 0;

    /* prefix = n² */
    seq_t prefix = (seq_t)n * (seq_t)n;

    /* iterate k = 1, 2, ... increasing the number of appended digits */
    seq_t pow10 = 10; /* 10ᵏ for current k */
    for (;;) {
        /* low  = prefix · 10ᵏ
           high = (prefix + 1)·10ᵏ – 1   (inclusive)          */
        seq_t low  = prefix * pow10;
        seq_t high = (prefix + 1) * pow10 - 1;

        /* integer lower bound of sqrt(low) (ceil) */
        seq_t s = floor_sqrt(low);
        if (s * s < low) s++;

        /* integer upper bound of sqrt(high) (floor) */
        seq_t e = floor_sqrt(high);

        /* check every candidate in the interval */
        for (seq_t m = s; m <= e; ++m) {
            seq_t sq = m * m;
            if ((sq % 100) != 0)        /* exclude squares ending '00' */
                return sq;
        }

        /* prepare next power of 10 for the next iteration */
        pow10 *= 10;
    }
}
