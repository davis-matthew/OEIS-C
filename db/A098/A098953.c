/*
 * A098953  Slowest increasing sequence where each number is such that at
 *          least one pair of adjacent digits are consecutive, the first of
 *          the pair being the smaller.
 * https://oeis.org/A098953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_consecutive_adjacent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth integer ≥12 whose decimal representation
   contains at least one pair of adjacent digits where the second digit
   is exactly one larger than the first.  The index n is 1‑based
   (a(1) = 12).  The implementation scans the decimal digits from
   most significant to least significant to test the adjacency condition. */
#include <stdbool.h>
#include "../utils/has_consecutive_adjacent.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* safety for out‑of‑range indices */
        return (seq_t)0;

    seq_t val = (seq_t)12;     /* first term of the sequence */
    idx_t count = 1;

    if (n == 1)
        return val;

    while (count < n) {
        ++val;
        if (has_consecutive_adjacent(val))
            ++count;
    }
    return val;
}
