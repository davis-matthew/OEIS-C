/*
 * A227078  The Ramanujan-Nagell squares: A038198(n)^2.
 * https://oeis.org/A227078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) + 7 = 2^A060728(n) (equivalently a(n) = (2*x-1)^2 where x
 *            solves x*(x-1)/2 + 1 = 2^z)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: full, pass, 5 terms through n = 4 (2026-11-02)
 * Helpers:   ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* The sequence is finite; after the 5 known solutions no terms are defined. */
    if (n >= 5)
        return 0;

    idx_t z = 0;
    idx_t found = 0;
    while (1) {
        /* Compute D = 2^(z+3) - 7 */
        seq_t D = pow2(z + 3);          /* 2^(z+3) */
        D -= 7;                         /* D = 2^(z+3) - 7 */

        /* Check if D is a perfect square */
        seq_t s = floor_sqrt(D);
        if (s * s == D) {
            if (found == n)
                return D;
            found++;
        }
        ++z;
    }
}
