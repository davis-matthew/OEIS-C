/*
 * A136485  Number of unit square lattice cells enclosed by origin centered
 *          circle of diameter n.
 * https://oeis.org/A136485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4 * Σ_{k=1..⌊n/2⌋} ⌊√((n/2)² – k²)⌋ (OEIS A136485 –
 *            number of unit squares fitting in a circle of diameter n,
 *            centered at the origin)
 * Range:     u8 n <= 18, u16 n <= 290, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* For n = 0 no terms are summed; but by construction n ≥ 1. */
    idx_t half   = n / 2;            /* ⌊n/2⌋ */
    seq_t  sum   = 0;                /* summation variable */

    /* Pre‑compute n² once; its value easily fits in 64‑bit for n ≤ 10⁴. */
    uint64_t n2  = (uint64_t)n * (uint64_t)n;

    for (idx_t k = 1; k <= half; ++k)
    {
        /* t = n² – 4·k² ≥ 0 for the permitted range of k. */
        uint64_t t   = n2 - 4 * (uint64_t)k * (uint64_t)k;

        /* Compute floor(√t) using double arithmetic, then correct if
           necessary to avoid round‑off errors. */
        unsigned long long s = (unsigned long long)floor(sqrt((double)t));
        while ((s + 1) * (s + 1) <= t) ++s;
        while (s * s > t)                --s;

        /* Each term contributes floor(√((n/2)² – k²)) = ⌊s/2⌋. */
        sum += (seq_t)(s >> 1);
    }

    /* Final result: 4 * Σ… */
    return sum << 2;   /* equivalent to sum * 4, but uses only integer ops */
}
