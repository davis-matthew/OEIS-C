/*
 * A187328  a(n) = floor((2-1/sqrt(2))*n); complement of A187338.
 * https://oeis.org/A187328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2-1/sqrt(2))*n)
 * Range:     u8 n <= 11, u16 n <= 120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 137 terms through n = 9097 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

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
    if (n == 0)
        return 0;                            /* a(0) = 0 */

    /* Promote n only when needed for multiplication. */
    unsigned __int128 sq   = (unsigned __int128)n * n; /* n² (huge-safe) */
    unsigned __int128 hsq  = sq / 2;                   /* floor(n²/2)   */

    /* ⌊n/√2⌋ = ⌊sqrt(floor(n²/2))⌋   */
    seq_t fp = floor_sqrt((seq_t)hsq);                 /* floor(n/√2)  */

    seq_t nn = (seq_t)n;
    /* a(n) = 2n − ⌊n/√2⌋ − 1  (holds for n > 0) */
    return 2 * nn - fp - 1;
}
