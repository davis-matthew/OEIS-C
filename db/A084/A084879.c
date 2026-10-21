/*
 * A084879  Number of (k,m,n)-multiantichains of multisets with k=3 and m=2.
 * https://oeis.org/A084879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9^n - 2*6^n + 3*3^n)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   pow_seq_iter
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow_seq_iter.h"

seq_t a(idx_t n) {
    seq_t nine  = 9;
    seq_t six   = 6;
    seq_t three = 3;

    seq_t pow9  = pow_seq_iter(nine,  n);
    seq_t pow6  = pow_seq_iter(six,   n);
    seq_t pow3  = pow_seq_iter(three, n);

    seq_t val = pow9 - 2 * pow6 + 3 * pow3;
    return val / 2;
}
