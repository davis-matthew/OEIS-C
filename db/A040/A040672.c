/*
 * A040672  Continued fraction for sqrt(699).
 * https://oeis.org/A040672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = term of continued fraction expansion of √699
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040672: continued fraction for sqrt(699) */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    const seq_t D = 699;
    seq_t a0 = floor_sqrt(D);
    if (n == 0) return a0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    for (idx_t i = 1; i <= n; ++i) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
    }
    return a;
}
