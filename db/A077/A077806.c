/*
 * A077806  Greatest prime factor of numbers containing in their decimal
 *          representation only the digits 0 and 1.
 * https://oeis.org/A077806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = greatest prime factor of the integer represented by the
 *            binary expansion of n, read as a decimal number of digits 0 and
 *            1 (i.e., a(n) = A006530(A007088(n)))
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined for n < 1 */

    /* Convert the binary representation of n into a decimal number
       consisting only of 0‑ and 1‑digits. */
    uint64_t val = 0;
    uint64_t tmp = n;
    unsigned len = 0;
    do {                     /* count bits of n */
        ++len;
        tmp >>= 1;
    } while (tmp);

    for (int i = (int)len - 1; i >= 0; --i) {
        val = val * 10 + ((n >> i) & 1);
    }

    if (val <= 1)
        return (seq_t)1;       /* greatest prime factor of 1 is 1 */

    return (seq_t)gpf(val);    /* gpf from utils: greatest prime factor */
}
