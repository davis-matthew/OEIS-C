/*
 * A156275  a(n) = 10^n*Catalan(n).
 * https://oeis.org/A156275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n * Catalan(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   catalan, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    return catalan(n) * pow10(n);
}
