/*
 * A147536  a(n) = 4*3^n+4^n.
 * https://oeis.org/A147536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4*3^n + 4^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t pow3 = pow_seq((seq_t)3, n);
    seq_t pow4 = pow_seq((seq_t)4, n);
    return 4 * pow3 + pow4;
}
