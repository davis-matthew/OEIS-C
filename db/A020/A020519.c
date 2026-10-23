/*
 * A020519  11th cyclotomic polynomial evaluated at powers of 2.
 * https://oeis.org/A020519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2^n + 4^n + 8^n + 16^n + 32^n + 64^n + 128^n + 256^n
 *            + 512^n + 1024^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= 10; ++k) {
        idx_t exp = k * n;
        sum += seq_pow((seq_t)2, exp);
    }
    return sum;
}
