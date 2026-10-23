/*
 * A013651  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains at least two 1's.
 * https://oeis.org/A013651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers k such that the period of the continued fraction for sqrt(k)
   contains at least two 1's.  Denote a(1)=7.
   This routine performs a linear search on the positive integers and
   counts those satisfying the condition.  The search stops as soon as the
   n-th such integer is found. */
   
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer floor square root of an unsigned 64‑bit value. */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 1, high = 0xFFFFFFFFULL; /* sqrt(2^64-1) < 2^32 */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Return true if the continued‑fraction period of sqrt(D) contains
   at least two 1's. */
static bool has_two_ones(idx_t D)
{
    idx_t a0 = isqrt_u64(D);
    if (a0 * a0 == D)              /* perfect square */
        return false;

    idx_t m = 0, d = 1, a = a0;
    int cnt = 0;
    for (;;) {
        m = d * a - m;
        unsigned __int128 t = (unsigned __int128)D - (unsigned __int128)m * m;
        d = (idx_t)(t / d);
        a = (a0 + m) / d;
        if (a == 1) ++cnt;
        if (cnt >= 2) return true;
        if (a == 2 * a0) break;    /* end of period */
    }
    return false;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for offset 1 */

    idx_t candidate = 2;           /* start testing from 2 */
    idx_t found = 0;
    while (found < n) {
        if (has_two_ones(candidate)) ++found;
        ++candidate;
    }
    return (seq_t)(candidate - 1);
}
