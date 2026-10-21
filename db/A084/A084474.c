/*
 * A084474  Write n in binary and replace 0 with 0000.
 * https://oeis.org/A084474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the decimal number obtained from the binary
 *            representation of n by replacing each 0 with 0000 and then
 *            interpreting the resulting string as a decimal integer
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n == 0) return 0;
    idx_t bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        bitlen++;
        tmp >>= 1;
    }
    while (bitlen--) {
        idx_t bit = (n >> bitlen) & 1;
        if (bit) {
            res = res * (seq_t)10 + 1;
        } else {
            res = res * (seq_t)10000;   /* append four zeros */
        }
    }
    return res;
}
