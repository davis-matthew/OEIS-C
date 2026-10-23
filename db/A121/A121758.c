/*
 * A121758  In decimal number system, take odd digits of n with negative sign.
 * https://oeis.org/A121758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign, nice, look
 * Formula:   a(n) = Σ_{i ≥ 0} (-1)^{d_i} · d_i · 10^i where d_i is the i-th
 *            decimal digit of n (least significant digit has i=0)
 * Range:     i8 n <= 129, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n = 0 the sequence is undefined (offset 1), but we handle it
       gracefully by returning 0. */
    if (n == 0) return (seq_t)0;

    __int128 result = 0;   /* use a large temporary type to avoid overflow */
    __int128 place  = 1;   /* 10^i */
    idx_t remaining = n;

    while (remaining > 0)
    {
        unsigned digit = (unsigned)(remaining % 10);
        __int128 sign   = (digit & 1) ? -1 : 1;   /* odd → -1, even → +1 */
        result += sign * digit * place;

        place *= 10;
        remaining /= 10;
    }

    return (seq_t)result;   /* truncate to the actual type; undefined only if
                               the value does not fit, which is permitted for
                               all terms that fit in seq_t */
}
