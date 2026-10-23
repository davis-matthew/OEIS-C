/*
 * A040967  Continued fraction for sqrt(999).
 * https://oeis.org/A040967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=31, a(n)=period[(n-1)%18] for n>=1,
 * where period={1,1,1,1,5,6,1,5,2,5,1,6,5,1,1,1,1,62}.
 * This yields the continued‑fraction coefficients of √999.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[18] = {
        1, 1, 1, 1, 5, 6, 1, 5,
        2, 5, 1, 6, 5, 1, 1, 1,
        1, 62
    };

    if (n == 0)
        return (seq_t)31;

    idx_t idx = (n - 1) % 18;
    return period[(size_t)idx];
}
