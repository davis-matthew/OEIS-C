/*
 * A073215  Sum of two powers of 23.
 * https://oeis.org/A073215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 23^i + 23^j, where i and j are chosen from the
 *            anti‑diagonal * enumeration: * n = i*(i+1)/2 + j, 0 ≤ j ≤ i. *
 *            This implements the definition of OEIS A073215
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 34, u64 n <= 118, u128 n <= 434,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Find i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2. */
    idx_t lo = 0, hi = 1;

    /* Increase hi until the bound is exceeded.  Use 128-bit arithmetic
     * to avoid overflow for large n. */
    while (((unsigned __int128)hi * (hi + 1)) >> 1 <= n) {
        if (hi > (idx_t)-1 >> 1)  /* guard against overflow of hi << 1 */
            break;
        hi <<= 1;
    }

    /* Binary search for i. */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 t = ((unsigned __int128)mid * (mid + 1)) >> 1;
        if (t <= n)
            lo = mid + 1;
        else
            hi = mid;
    }

    idx_t i = lo - 1;
    unsigned __int128 prev = ((unsigned __int128)i * (i + 1)) >> 1;
    idx_t j = n - (idx_t)prev;          /* 0 ≤ j ≤ i */

    seq_t pow_i = pow_int((seq_t)23, i);
    seq_t pow_j = pow_int((seq_t)23, j);

    return pow_i + pow_j;
}
