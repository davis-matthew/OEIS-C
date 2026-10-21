/*
 * A095549  a(n) = 78 written in base 15 - n.
 * https://oeis.org/A095549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal value of the base (15 – n) representation of the
 *            * decimal number 78. For positive bases the usual base
 *            conversion is used. * For base 1 a unary (78 ones) is produced.
 *            For negative bases with |b|>1 we * use the standard algorithm
 *            for negative base representation. For base 0 * (n =...
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   convert_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/convert_base.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute base = 15 – n as a signed integer */
    int64_t base = (int64_t)15 - (int64_t)n;
    if (base == 0)   /* n == 15, invalid base */
        return 0;
    return convert_base((int64_t)78, base);
}
