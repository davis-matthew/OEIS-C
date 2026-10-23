/*
 * A162472  Rewrite n in binary with each digit occurring n times (see
 *          example). a(n) is the decimal equivalent.
 * https://oeis.org/A162472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the decimal number obtained by writing each bit of * the
 *            binary representation of n repeated n times
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Compute the nth term of OEIS A162472.  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* 2^n - 1 in seq_t */
    seq_t ones = pow2(n) - 1;

    /* position of most significant set bit of n (0‑based) */
    idx_t msb = 0;
    for (idx_t t = n; t > 1; t >>= 1)
        ++msb;

    seq_t result = 0;

    /* Process bits from MSB to LSB, appending a block of n bits
     * for each original bit. */
    for (int i = msb; i >= 0; --i) {
        result <<= n;
        if ((n >> i) & 1)
            result += ones;
    }

    return result;
}
