/*
 * A075439  Triangle read by rows giving successive iterations of the Rule 102
 *          elementary cellular automaton starting with a single ON cell where
 *          row n is of length 2n+1.
 * https://oeis.org/A075439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = C( floor(sqrt(n)), n - floor(sqrt(n))^2 ) (mod 2)
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor sqrt for a 64‑bit unsigned integer.  */
static inline idx_t
u64_floor_sqrt(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1ULL << 32;   /* hi > sqrt(UINT64_MAX) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t
a(idx_t n)
{
    idx_t r = u64_floor_sqrt(n);      /* floor(sqrt(n))              */
    idx_t d = n - r * r;              /* offset within row (0‑based) */

    /* binomial coefficient C(r, d) is odd iff (d & ~r) == 0  */
    return ((d & ~r) == 0) ? (seq_t)1 : (seq_t)0;
}
