/*
 * A147568  a(n) = 2*A000695(n)+3.
 * https://oeis.org/A147568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A000695(n)+3
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000695(n) is obtained by interpreting the binary digits of n as coefficients of powers of 4. */
#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t bit = 0;
    idx_t temp = n;
    while (temp) {
        if (temp & 1) {
            /* 4^bit = 1 << (2*bit) */
            res += ((seq_t)1) << (idx_t)(2 * bit);
        }
        temp >>= 1;
        ++bit;
    }
    return (res << 1) + 3;
}
