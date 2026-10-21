/*
 * A073122  Minimal reversing binary representation of n. Converting sum of
 *          powers of 2 in binary representation of a(n) to alternating sum
 *          gives n. See A072339.
 * https://oeis.org/A073122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(2n) = 2*a(n). For any n that is not a power of two, let p be
 *            the smallest power of two strictly greater than n. Then a(n) = p
 *            + a(p - n). This recurrence generates the unique minimal
 *            reversing binary representation of n
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a_internal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/a_internal.h"

/* Public function required by the build */
seq_t a(idx_t n)
{
    return a_internal(n);
}
