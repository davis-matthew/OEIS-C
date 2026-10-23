/*
 * A187916  a(n) = C(n) if n is odd, else C(n) - C(n/2); C(n) are Catalan
 *          numbers.
 * https://oeis.org/A187916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = catalan(n) if n is odd, else catalan(n) - catalan(n/2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t c = catalan(n);
    if ((n & 1) == 0) {
        c -= catalan(n / 2);
    }
    return c;
}
