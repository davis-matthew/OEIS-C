/*
 * A170229  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 28, a(n) = 27 * a(n-1) for n ≥ 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   power27
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power27.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 28;
    /* for n ≥ 2: a(n) = 28 * 27^(n-1) */
    return 28 * power27(n - 1);
}
