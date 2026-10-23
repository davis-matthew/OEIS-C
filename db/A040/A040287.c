/*
 * A040287  Continued fraction for sqrt(305).
 * https://oeis.org/A040287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=floor(sqrt(305))=17; for n≥1, a(n) is given by the standard
 *            recurrence for the continued fraction of sqrt(305): let m₀=0,
 *            d₀=1 and a₀=floor(sqrt(305)). For k≥0: mₖ₊₁ = dₖ·aₖ − mₖ dₖ₊₁ =
 *            (305 − mₖ₊₁²) / dₖ aₖ₊₁ = ⌊(a₀ + mₖ₊₁) / dₖ₊₁⌋ The resulting
 *            sequence is 17, 2, 6, 2, 34, 2, 6, 2, 34, … (...
 * Range:     u8 none, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    const seq_t D = 305;
    seq_t a0 = floor_sqrt(D);
    if (n == 0)
        return a0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a_cur = a0;
    idx_t i;

    for (i = 1; i <= n; ++i) {
        /* compute next term in the continued fraction */
        m = d * a_cur - m;
        d = (D - m * m) / d;
        a_cur = (a0 + m) / d;
        if (i == n)
            return a_cur;
    }
    /* unreachable but keep compiler happy */
    return a_cur;
}
