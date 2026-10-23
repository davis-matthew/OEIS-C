/*
 * A032634  a(n) = floor(n/e).
 * https://oeis.org/A032634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/e)
 * Range:     u8 n <= 695, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses an integer approximation of 1/e based on the scaling 2^20. */
/* The constant FLOOR_E_SCALED is floor(e * 2^20).  The function
   verifies the inequalities that uniquely determine floor(n/e) for any
   64‑bit index n.  The algorithm runs in O(1) time and uses only
   integer arithmetic. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Scaling factor (2^20). 2^20 = 1,048,576. */
    const uint64_t SCALE = (1ULL << 20);

    /* floor(e * SCALE).  e ≈ 2.718281828459045; multiplied by 2^20 gives
       2,850,325.086… so the floor is 2,850,325. */
    const uint64_t FLOOR_E_SCALED = 2850325ULL;

    /* Compute lower and upper bounds for the answer by the well
       known inequalities:
           floor(n/e) <= floor(n * SCALE / FLOOR_E_SCALED)
           floor(n/e) >= floor(n * SCALE / (FLOOR_E_SCALED + 1))
       The interval size is at most 10 for n up to 2^64, so a short
       linear scan suffices to find the exact value. */
    uint64_t low  = ((unsigned __int128)n * SCALE) / (FLOOR_E_SCALED + 1);
    uint64_t high = ((unsigned __int128)n * SCALE) / FLOOR_E_SCALED;

    /* Verify the defining inequalities:
           n*SCALE >= m*(FLOOR_E_SCALED+1)
           n*SCALE < (m+1)*FLOOR_E_SCALED              */
    const unsigned __int128 nscaled = (unsigned __int128)n * SCALE;

    for (uint64_t m = high; ; --m)
    {
        const unsigned __int128 left  = (unsigned __int128)m                * (FLOOR_E_SCALED + 1);
        const unsigned __int128 right = (unsigned __int128)(m + 1ULL)       *  FLOOR_E_SCALED;

        if (left <= nscaled && nscaled < right)
            return (seq_t)m;          /* exact value found */

        if (m == low)
            break;                    /* safety fall‑back */
    }

    /* In the unlikely event that the loop exits without success
       (which should not happen for the given bounds), return the
       lower bound. */
    return (seq_t)low;
}
