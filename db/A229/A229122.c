/*
 * A229122  For odd m, let f(m) be the odd part of 3*m+1. a(n) is the least
 *          positive number of f-iterations of 2*n-1 to reach an odious number
 *          (A000069), or 0 if no such number of f-iterations exists.
 * https://oeis.org/A229122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least positive number of f‑iterations of 2*n-1
 *            needed to reach an odious number (A000069). It is computed by
 *            iterating the odd part map f(m) = odd_part(3*m+1) and using
 *            Floyd’s cycle detection to detect infinite loops. If no such
 *            iteration exists, the function returns 0
 * Range:     u8 n <= 27, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_odious
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_odious.h"

/* Odd part of 3*m+1 for an odd m.  Works for built‑in integers
   and for arbitrary‑precision seq_t objects that support the
   usual operators. */
static inline seq_t odd_part(seq_t m)
{
    seq_t y = 3 * m + 1;
    while ((y & 1) == 0)
        y >>= 1;
    return y;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* 2*n-1 is odd.  Cast n to seq_t without overflow. */
    seq_t x = (seq_t)2 * (seq_t)n - 1;

    /* Floyd’s cycle detection: tortoise moves one step,
       hare moves two steps.  steps counts how many
       f‑applications have been performed.  */
    seq_t tortoise = x;
    seq_t hare     = x;
    seq_t steps    = 0;

    /* Safety limit to avoid infinite loops in practice. */
    const seq_t MAX_STEPS = 200000;

    while (steps < MAX_STEPS) {
        /* Advance tortoise by one iteration. */
        ++steps;
        tortoise = odd_part(tortoise);

        /* If the result is odious, return the step count. */
        if (is_odious(tortoise))
            return steps;

        /* Advance hare by two iterations. */
        hare = odd_part(hare);
        hare = odd_part(hare);

        /* If tortoise and hare meet, a cycle has been found. */
        if (hare == tortoise)
            return (seq_t)0;
    }

    /* If the limit is exceeded, assume no finite result. */
    return (seq_t)0;
}
