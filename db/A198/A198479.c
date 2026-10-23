/*
 * A198479  a(n) = 10^n * n^10.
 * https://oeis.org/A198479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n * n^10
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 24,
 *            big any n
 * Helpers:   pow10, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t pow10n = pow10(n);
    seq_t npow10 = pow_int((seq_t)n, (idx_t)10);
    return pow10n * npow10;
}
