/*
 * A029930  If 2n = Sum 2^e_i, a(n) = Product 2^e_i.
 * https://oeis.org/A029930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = 2^{∑_{k=0}^{∞} (k+1)*bit_k(n)} where bit_k(n) is the k-th
 *            binary digit of n (LSB has k = 0). Equivalent to the definition:
 *            a(0) = 1, a(2n) = 2^{popcount(n)} * a(n), a(2n+1) = 2*a(2n)
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 246, u64 n <= 2044,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the sum of (k+1) over all set bits of n. */
    uint64_t sum = 0;
    idx_t tmp = n;
    uint64_t pos = 1;               /* position counter, starts at 1 for the LSB of 2n */
    while (tmp) {
        if (tmp & 1)
            sum += pos;
        tmp >>= 1;
        pos++;
    }

    /* Compute 2^sum, safe for all seq_t sizes because we multiply by 2
       sum times. For small sum this is inexpensive. */
    seq_t result = 1;
    while (sum--) {
        result = result * (seq_t)2;
    }
    return result;
}
