/*
 * A001550  a(n) = 1^n + 2^n + 3^n.
 * https://oeis.org/A001550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1^n + 2^n + 3^n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow.h"

seq_t a(idx_t n) {
    return (seq_t)1 + ipow((seq_t)2, n) + ipow((seq_t)3, n);
}
