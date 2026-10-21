/*
 * A232243  a(n) = wt(n^2) - wt(n), where wt(n) = A000120(n) is the binary
 *          weight function.
 * https://oeis.org/A232243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = wt(n^2) - wt(n) (wt = binary weight, A000120)
 * Range:     i8 n <= 15, i16 n <= 1622, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t popcount_seq(seq_t x)
{
    seq_t cnt = 0;
    while (x) {
        cnt++;
        x &= (x - 1);          /* clear lowest set bit */
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t sq   = sn * sn;
    seq_t wsq  = popcount_seq(sq);
    seq_t wn   = popcount_seq(sn);
    return wsq - wn;
}
