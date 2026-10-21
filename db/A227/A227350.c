/*
 * A227350  Product of lengths of runs of 0-bits in binary representation of
 *          n, or 1 if no nonleading zeros present.
 * https://oeis.org/A227350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = product of lengths of runs of 0-bits in the binary
 *            representation of n; a(0) = 1 (empty product). If n has no
 *            non-leading zeros (i.e., n = 2^k-1), result is 1
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* Find the position of the most significant 1-bit (0-indexed). */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++msb;
    }

    seq_t product = (seq_t)1;
    bool seen_one      = false;
    bool in_zero_run   = false;
    idx_t zero_len     = 0;

    for (idx_t pos = msb; pos <= msb; --pos) {
        bool bit = (n >> pos) & 1;
        if (bit) {
            seen_one = true;
            if (in_zero_run) {
                product *= (seq_t)zero_len;
                zero_len = 0;
                in_zero_run = false;
            }
        } else {
            if (seen_one) {
                in_zero_run = true;
                ++zero_len;
            }
        }
        if (pos == 0) break; /* guard against underflow when pos becomes negative */
    }

    if (in_zero_run) {
        product *= (seq_t)zero_len;
    }

    return product;
}
