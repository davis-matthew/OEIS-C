/*
 * A028352  A Golomb-like recurrence that decreases infinitely often.
 * https://oeis.org/A028352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12 * 3^⌊log_3(n/2)⌋ - 2n Offset: 1 (a(1) = 2) The
 *            recurrence a(2*3^m + r) = 8*3^m - 2r (0 ≤ r ≤ 4·3^m – 1) gives
 *            the same result. For n = 1 the above formula would need a floor
 *            of a negative logarithm, so we handle that case explicitly. For
 *            all other n the integer division n/2 is sufficien...
 * Range:     u8 n <= 161, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_log3_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log3_idx.h"

seq_t a(idx_t n)
{
    if (n == 1)   /* special case: a(1) = 2 */
        return 2;

    idx_t half = n / 2;                    /* floor n/2  (integer division) */
    idx_t e = floor_log3_idx(half);        /* ⌊log_3(n/2)⌋ */

    seq_t pow3 = 1;
    for (idx_t i = 0; i < e; ++i)
        pow3 *= (seq_t)3;                  /* 3^e in seq_t */

    seq_t term = (seq_t)12 * pow3 - (seq_t)2 * (seq_t)n;
    return term;
}
