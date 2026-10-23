/*
 * A174212  Ultradoublefactorials: a(n) = (n!!)^(n!!).
 * https://oeis.org/A174212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n!!)^(n!!)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

static seq_t
double_factorial(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = n; i > 1; i -= 2)
        res *= (seq_t)i;
    return res;
}

seq_t
a(idx_t n)
{
    seq_t d = double_factorial(n);
    return big_pow(d, d);
}
