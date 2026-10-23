/*
 * A177725  Number of line segments connecting exactly 9 points in an n x n
 *          grid of points.
 * https://oeis.org/A177725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{a,b ≥ 0, (a,b) ≠ (0,0), gcd(a,b)=8} * (n-a)·(n-b) · (
 *            (a==0 || b==0) ? 1 : 2 ) * * This counts unordered pairs of grid
 *            points whose line segment * contains exactly 9 points, i.e.
 *            whose coordinate differences * have gcd equal to 8
 * Range:     u8 n <= 14, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Euclidean gcd for idx_t (uint64_t) */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n <= 1 yields no interior grid points to form a segment */
    if (n < 9)
        return 0;

    idx_t maxk = (n - 1) / 8;          /* maximum value of a' or b' */
    seq_t result = 0;

    for (idx_t ai = 0; ai <= maxk; ++ai)
    {
        for (idx_t bi = 0; bi <= maxk; ++bi)
        {
            if (ai == 0 && bi == 0)
                continue;
            if (gcd_idx(ai, bi) != 1)
                continue;

            idx_t a = (idx_t)8 * ai;
            idx_t b = (idx_t)8 * bi;

            /* (n-a) and (n-b) are at most n <= 10000, fit in idx_t */
            seq_t term = (seq_t)(n - a) * (seq_t)(n - b);

            /* Multiply by 2 if both a and b are non‑zero */
            if (a != 0 && b != 0)
                term = term * (seq_t)2;

            result += term;
        }
    }

    return result;
}
