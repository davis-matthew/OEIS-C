/*
 * A096891  Least hypotenuse of primitive Pythagorean triangles with odd leg
 *          2n+1.
 * https://oeis.org/A096891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = min_{d|(2n+1), d < (2n+1)/d, gcd(d,(2n+1)/d) = 1} (d² +
 *            ((2n+1)/d)²)/2 * (Least value of x² + y² with gcd(x,y)=1 and y²
 *            – x² = 2n+1) * This implementation enumerates all factor pairs
 *            of 2n+1 and selects the one * that yields the smallest
 *            hypotenuse squared
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Y = 2n + 1 */
    seq_t Y = (seq_t)n * 2 + 1;
    /* For the intended ranges n fits in 64 bits, so we convert to uint64_t */
    uint64_t y = (uint64_t)Y;

    seq_t best = 0;
    bool found = false;
    for (uint64_t d = 1; d * d <= y; ++d) {
        if (y % d != 0) continue;
        uint64_t d2 = y / d;
        if (d >= d2) continue;          /* ensure d < d2 */
        if (gcd((seq_t)d, (seq_t)d2) != 1) continue;
        seq_t d_sq  = (seq_t)d * (seq_t)d;
        seq_t d2_sq = (seq_t)d2 * (seq_t)d2;
        seq_t val = (d_sq + d2_sq) / 2;
        if (!found || val < best) {
            best = val;
            found = true;
        }
    }
    return best;
}
