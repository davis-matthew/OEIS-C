/*
 * A066621  Concatenation of triangular numbers in increasing order up to the
 *          n-th and then in decreasing order.
 * https://oeis.org/A066621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   concatenate the triangular numbers T_k = k*(k+1)/2 in the order
 *            T1, T2, …, Tn, T_{n-1}, …, T1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

static int digit_len_seq(seq_t x) {
    int len = 0;
    while (x > 0) {
        x /= 10;
        ++len;
    }
    return len;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t tri;
    int digs;
    idx_t k;

    /* ascending part 1..n */
    for (k = 1; k <= n; ++k) {
        tri = ((seq_t)k * ((seq_t)k + 1)) / 2;
        digs = digit_len_seq(tri);
        result = result * pow10((idx_t)digs) + tri;
    }

    /* descending part n-1..1 */
    for (k = n - 1; k > 0; --k) {
        tri = ((seq_t)k * ((seq_t)k + 1)) / 2;
        digs = digit_len_seq(tri);
        result = result * pow10((idx_t)digs) + tri;
    }

    return result;
}
