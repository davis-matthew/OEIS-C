/*
 * A037084  Positive integers not going to 1 under iterations of the map in
 *          A001281: n->3n-1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1027, u32 n <= 8584, u64 n <= 8584,
 *            u128 n <= 8584, big any n
 * Helpers:   reaches_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A037084: positive integers that never reach 1 under
   the iteration f(n) = 3*n-1 if n is odd, n/2 if n is even. */

#include <stdbool.h>
#include "../utils/reaches_one.h"

/* Main sequence function.  a(1) is the first positive integer that does
   not reach 1 under the given map. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, this case is unused */

    idx_t count = 0;
    seq_t candidate = (seq_t)1;

    while (1) {
        if (!reaches_one(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
