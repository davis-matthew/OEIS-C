/*
 * A103333  Number of closed walks on the graph of the (7,4) Hamming code.
 * https://oeis.org/A103333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(n)=8*a(n-1) for n>0 (Equivalently, a(n)=3*8^(n-1) for
 *            n>0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* Start with a(1)=3 and multiply by 8 for each step until reaching a(n). */
    seq_t val = (seq_t)3;
    for (idx_t i = 1; i < n; ++i) {
        val = val * (seq_t)8;
    }
    return val;
}
