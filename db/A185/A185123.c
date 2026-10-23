/*
 * A185123  a(n) = n 9's sandwiched between two 1's.
 * https://oeis.org/A185123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less, base
 * Formula:   a(n) = 20*10^n - 9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t v = pow10(n);        /* v = 10^n */
    v = v * (seq_t)20;         /* v = 20*10^n */
    v = v - (seq_t)9;          /* v = 20*10^n - 9 */
    return v;
}
