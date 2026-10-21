/*
 * A054072  Position of n in the permutation of 1,2,...,n obtained by ordering
 *          the fractional parts {h*sqrt(2)} for h=1,2,...,n.
 * https://oeis.org/A054072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + #{h : 1 ≤ h < n and ⌊n·√2⌋ - ⌊h·√2⌋ ≤ ⌊(n-h)·√2⌋}
 * Range:     u8 n <= 5, u16 n <= 69, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the ordering of fractional parts {h·√2} */

/* Compute ⌊k·√2⌋ by using an integer square root:
   ⌊k·√2⌋ = ⌊√(2·k²)⌋.  */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1U) {
        return 1;   /* a(1) = 1 */
    }

    /* Precompute ⌊n·√2⌋ */
    seq_t fn = floor_sqrt((seq_t)2 * (seq_t)n * (seq_t)n);

    seq_t pos = 1; /* position of n itself */

    for (idx_t h = 1; h < n; ++h) {
        seq_t fh = floor_sqrt((seq_t)2 * (seq_t)h * (seq_t)h);
        seq_t diff = n - h;
        seq_t fl = floor_sqrt((seq_t)2 * (seq_t)diff * (seq_t)diff);

        if ((fn - fh) <= fl) {
            ++pos;
        }
    }

    return pos;
}
