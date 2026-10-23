/*
 * A024093  a(n) = 8^n - n^5.
 * https://oeis.org/A024093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - n^5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Compute 8^n and n^5.  pow_seq works for seq_t bases and
       small integer exponents. */
    seq_t pow8   = pow_seq((seq_t)8, (int)n);
    seq_t pown5  = pow_seq((seq_t)n, 5);
    return pow8 - pown5;
}
