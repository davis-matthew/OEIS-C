/*
 * A087263  a(n) is the least initial value of a 3x+1 trajectory in which n is
 *          the largest (peak) term or a(n) = 0 if n cannot be a peak value
 *          (i.e., when n = 2k+1, n = 4k+2, n = 16k+12, etc.).
 * https://oeis.org/A087263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A087263: least initial value of a Collatz
   (3x+1) trajectory whose maximum (peak) term equals n.
   Returns 0 if no such start exists. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    for (idx_t start = 1; start <= n; ++start) {
        seq_t cur  = (seq_t)start;
        seq_t peak = cur;
        while (cur != 1) {
            if (cur & 1) {
                cur = cur * 3 + 1;
            } else {
                cur >>= 1;
            }
            if (cur > peak) peak = cur;
        }
        if (peak == (seq_t)n) {
            return (seq_t)start;
        }
    }
    return 0;
}
