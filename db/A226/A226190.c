/*
 * A226190  Least positive integer k such that 1 + 1/2 + ... + 1/k >= log(n).
 * https://oeis.org/A226190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer k such that 1 + 1/2 + … + 1/k >=
 *            log(n)
 * Range:     u8 n <= 455, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integer operations on seq_t.  Positional index n is of type idx_t. */

#include <math.h>    /* for log */

seq_t a(idx_t n)
{
    /* Target real value: natural log of the argument */
    double target = log((double)n);

    /* Incrementally build the harmonic series until it reaches or exceeds target */
    double h = 0.0;
    idx_t k = 0;

    for (;;)
    {
        ++k;
        h += 1.0 / (double)k;
        if (h >= target)
            return (seq_t)k;
    }
}
