/*
 * A043713  Numbers whose base-10 representation has an odd number of runs.
 * https://oeis.org/A043713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A043713 Numbers whose base‑10 representation has an odd number of runs.
   This implementation returns the nth such number by a simple linear search. */

#include <stdbool.h>
#include "../utils/odd_runs.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* value for offset 0 (unused) */

    idx_t count = 0;
    idx_t candidate = 0;
    while (count < n)
    {
        ++candidate;
        if (odd_runs(candidate))
            ++count;
    }
    return (seq_t)candidate;
}
