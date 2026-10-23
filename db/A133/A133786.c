/*
 * A133786  Numbers prime to 10, 10's digit is odd and 100's digit is odd.
 * https://oeis.org/A133786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 6546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers prime to 10, 10's digit is odd and 100's digit is odd
   a(n) = 100*(1+2*⌊(n-1)/20⌋) + 10*(1+2*⌊((n-1)%20)/4⌋) + {1,3,7,9}[ (n-1)%4 ]
   This follows from the periodicity a(n+20)=a(n)+200.   */
#include <stdint.h>

seq_t a(idx_t n) {
    /* n must be ≥ 1 (offset 1).  For n < 1 the result is undefined. */
    idx_t m = n - 1;           /* zero‑based index */
    idx_t block = m / 20;      /* block number */
    idx_t pos = m % 20;        /* position inside the block */

    /* hundreds digit: 1,3,5,7,9,...  i.e. 1 + 2*block */
    seq_t hundreds = (seq_t)(1 + ((seq_t)block << 1));

    /* tens digit: 1,3,5,7,9 repeated every 4 positions */
    seq_t tens = (seq_t)(1 + 2 * (pos / 4));

    /* ones digit: cycle 1,3,7,9 */
    static const unsigned char ones_list[4] = {1, 3, 7, 9};
    seq_t ones = (seq_t)ones_list[pos % 4];

    /* combine the digits */
    return hundreds * 100 + tens * 10 + ones;
}
