/*
 * A033654  Trajectory of 17 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 17, a(n+1) = a(n) + reverse_decimal(a(n))
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n) {
    seq_t cur = 17;          /* a(0) */
    if (n == 0) return cur;
    for (idx_t i = 0; i < n; ++i) {
        seq_t rev = reverse_decimal(cur);
        cur = cur + rev;
    }
    return cur;
}
