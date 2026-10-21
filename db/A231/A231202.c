/*
 * A231202  The smallest possible speed in m/s (rounded up) of an object whose
 *          relativistic mass is n times its rest mass.
 * https://oeis.org/A231202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(299792458*sqrt(1 - 1/n^2))
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = ceil(C * sqrt(1 - 1/n^2)) with C = 299792458 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    const uint64_t C    = 299792458ULL;
    const __int128 C2   = (__int128)C * C;
    const __int128 n2   = (__int128)n * n;
    const __int128 rhs  = C2 * (n2 - 1);  /* RHS = C^2 * (n^2 - 1) */

    seq_t low  = 0;
    seq_t high = (seq_t)C;                 /* high is an upper bound: a(n) ≤ C */

    while (low < high) {
        seq_t mid = low + (high - low) / 2;

        __int128 prod = (__int128)mid * (__int128)n;
        __int128 lhs  = prod * prod;       /* (mid * n)^2 */

        if (lhs >= rhs)            /* condition: (mid*n)^2 ≥ C^2*(n^2-1) */
            high = mid;
        else
            low = mid + 1;
    }

    return high;
}
