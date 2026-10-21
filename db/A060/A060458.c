/*
 * A060458  Maximum value seen in the final n decimal digits of 2^j for all
 *          values of j.
 * https://oeis.org/A060458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 10^n - 2^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   pow10, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(n) = pow10(n) - pow2(n)  for n ≥ 1 */
    return pow10(n) - pow2(n);
}
