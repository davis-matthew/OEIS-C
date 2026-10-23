/*
 * A043744  Numbers whose number of runs in the base-2 representation is
 *          congruent to 5 mod 7.
 * https://oeis.org/A043744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   binary_run_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th positive integer whose base‑2 representation
 * contains a number of runs (consecutive equal bits) congruent to 5 modulo 7.
 * a(1) = 21, a(2) = 37, a(3) = 41, …
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binary_run_count.h"

/* Return the n‑th element of the sequence.  The index is 1‑based. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (uint64_t m = 1ULL; ; ++m)
    {
        if (binary_run_count(m) % 7 == 5)
        {
            ++found;
            if (found == n)
                return (seq_t)m;     /* Cast to seq_t type */
        }
    }
}
