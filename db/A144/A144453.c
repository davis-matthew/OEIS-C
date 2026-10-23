/*
 * A144453  a(n) = A061039(8*n+5).
 * https://oeis.org/A144453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numerator of reduced fraction 16*(n+1)*(4*n+1) /
 *            (9*(8*n+5)^2)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = (seq_t)16 * (nn + 1) * (4 * nn + 1);
    seq_t den = (seq_t)9 * (8 * nn + 5) * (8 * nn + 5);
    seq_t g = seq_gcd(num, den);
    return num / g;
}
