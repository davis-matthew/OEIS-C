/*
 * A040836  Continued fraction for sqrt(866).
 * https://oeis.org/A040836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 29; for n ≥ 1, a(n) = [2, 2, 1, 28, 1, 2, 2, 58][(n‑1)
 *            mod 8]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)29;

    static const seq_t period[8] = {
        (seq_t)2, (seq_t)2, (seq_t)1,
        (seq_t)28, (seq_t)1, (seq_t)2,
        (seq_t)2, (seq_t)58
    };

    /* (n-1) is safe because n>0 here */
    size_t idx = (size_t)((n - 1) % 8);
    return period[idx];
}
