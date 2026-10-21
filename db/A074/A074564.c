/*
 * A074564  a(n) = 4^n + 5^n + 9^n.
 * https://oeis.org/A074564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 5^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   pow_int_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int_seq.h"

seq_t a(idx_t n) {
    seq_t p4 = pow_int_seq((seq_t)4, n);
    seq_t p5 = pow_int_seq((seq_t)5, n);
    seq_t p9 = pow_int_seq((seq_t)9, n);
    return p4 + p5 + p9;
}
