/*
 * A103459  a(n) = 8^n + 1 - 0^n.
 * https://oeis.org/A103459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^n + 1 for n > 0; a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    seq_t pow = seq_pow((seq_t)8, n);
    return pow + (seq_t)1;
}
