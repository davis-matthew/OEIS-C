/*
 * A007701  a(0) = 0; for n > 0, a(n) = n^n*2^((n-1)^2).
 * https://oeis.org/A007701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n * 2^((n-1)^2) for n >= 1, a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   ipow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_seq.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t m = n - 1;
    seq_t p = ipow_seq((seq_t)n, n);
    seq_t q = ipow_seq((seq_t)2, m * m);
    return p * q;
}
