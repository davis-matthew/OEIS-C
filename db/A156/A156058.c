/*
 * A156058  a(n) = 5^n * Catalan(n).
 * https://oeis.org/A156058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n * Catalan(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   catalan, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    return power5(n) * catalan(n);
}
