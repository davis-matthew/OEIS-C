/*
 * A131921  Years between consecutive Leap Days for Gregorian calendar.
 * https://oeis.org/A131921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n+97) = a(n).  a(1)=8; a(n)=4 except at positions n≡1,25,74 (mod 97), where a(n)=8. */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t idx = (n - 1) % 97;
    if (idx == 0 || idx == 24 || idx == 73)
        return (seq_t)8;
    return (seq_t)4;
}
