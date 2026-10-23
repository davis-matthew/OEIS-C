/*
 * A040525  Continued fraction for sqrt(549).
 * https://oeis.org/A040525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=23; for n≥1, a(n)=cycle[(n‑1) mod 18] with cycle =
 *            {2,3,9,11,1,1,1,1,4,1,1,1,1,11,9,3,2,46}
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t cycle[18] = {
        2, 3, 9, 11,
        1, 1, 1, 1,
        4,
        1, 1, 1, 1,
        11, 9, 3, 2, 46
    };

    if (n == 0)
        return (seq_t)23;

    idx_t idx = (n - 1) % (idx_t)18;
    return cycle[(size_t)idx];
}
