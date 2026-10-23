/*
 * A178754  a(n) = n XOR floor(Log_2(n)), where XOR is bitwise XOR.
 * https://oeis.org/A178754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n XOR floor(log₂(n))
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   _log2_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_log2_floor.h"

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 */
    seq_t val = (seq_t)n;
    seq_t l2  = (seq_t)_log2_floor(n);
    return val ^ l2;
}
