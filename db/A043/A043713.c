/*
 * A043713  Numbers whose base-10 representation has an odd number of runs.
 * https://oeis.org/A043713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8333 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A043713 Numbers whose base‑10 representation has an odd number of runs.
   This implementation returns the nth such number by a simple linear search. */

#include <stdbool.h>

static inline bool odd_runs(idx_t x)
{
    idx_t prev = x % 10;
    x /= 10;
    int runs = 1;
    while (x > 0)
    {
        idx_t d = x % 10;
        if (d != prev)
            ++runs;
        prev = d;
        x /= 10;
    }
    return (runs & 1) != 0;
}

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
