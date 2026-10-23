/*
 * A164996  (Length of preperiodic part) + (length of cycle) of trajectory of
 *          n under iteration of the base-3 Kaprekar map in A164993.
 * https://oeis.org/A164996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 454, u16 n <= 2187, u32 n <= 2187, u64 n <= 2187,
 *            u128 n <= 2187, big any n
 * Helpers:   next_kaprekar
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164996: Length of preperiodic part + length of cycle of the base‑3
   Kaprekar map.  This implementation uses Floyd's cycle‑finding
   algorithm so that it works for arbitrary‑size seq_t values and
   remains O(1) memory. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_kaprekar.h"

/* Main sequence function. */
seq_t
a(idx_t n)
{
    seq_t start = (seq_t)n;

    /* Floyd's cycle detection. */
    seq_t tortoise = next_kaprekar(start);
    seq_t hare = next_kaprekar(next_kaprekar(start));

    /* Find first meeting point. */
    while (tortoise != hare) {
        tortoise = next_kaprekar(tortoise);
        hare     = next_kaprekar(next_kaprekar(hare));
    }

    /* Find preperiod length mu. */
    size_t mu = 0;
    tortoise = start;
    while (tortoise != hare) {
        tortoise = next_kaprekar(tortoise);
        hare     = next_kaprekar(hare);
        ++mu;
    }

    /* Find cycle length lambda. */
    size_t lambda = 1;
    hare = next_kaprekar(tortoise);
    while (tortoise != hare) {
        hare = next_kaprekar(hare);
        ++lambda;
    }

    return (seq_t)mu + (seq_t)lambda;
}
