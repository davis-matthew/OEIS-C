/*
 * A046233  Numbers whose cube is palindromic in base 5.
 * https://oeis.org/A046233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(1) = 0, a(2) = 1, a(n) = 5^(n-2) + 1 for n ≥ 3
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* n ≥ 3 : compute 5^(n-2) + 1 */
    idx_t exp = n - 2;
    seq_t val = seq_pow((seq_t)5, exp);
    val += (seq_t)1;
    return val;
}
