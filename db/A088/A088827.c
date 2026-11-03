/*
 * A088827  Even numbers with odd abundance: even squares or two times
 *          squares.
 * https://oeis.org/A088827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 8830 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088827: Even numbers with odd abundance.
   A(n) is the nth smallest integer of the form 2·k² or 4·k² for k ≥ 1.
   This can be obtained by binary searching on the integer x and counting
   how many such numbers are ≤ x:
        count(x) = ⌊√(x/2)⌋ + ⌊√(x/4)⌋ .
   The minimal x with count(x) ≥ n is the desired value.  */

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
    if (n == 0) return 0;            /* undefined for n < 1, but harmless */

    seq_t low  = (seq_t)2;                 /* first term is 2       */
    seq_t high = (seq_t)2 * (seq_t)n * (seq_t)n;  /* safe upper bound  */

    while (low < high) {
        seq_t mid = low + (high - low) / 2;

        idx_t cnt = (idx_t)floor_sqrt(mid / (seq_t)2)
                  + (idx_t)floor_sqrt(mid / (seq_t)4);

        if (cnt >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}
