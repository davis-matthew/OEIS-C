/*
 * A020333  Numbers whose base-5 representation is the juxtaposition of two
 *          identical strings.
 * https://oeis.org/A020333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*(5^floor(log_5(n)+1)+1)
 * Range:     u8 n <= 9, u16 n <= 124, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

extern seq_t a(idx_t n) {
    /* compute 5^k where k = floor(log_5(n)) + 1 */
    seq_t p = 1;
    while (p <= (seq_t)n) {
        p *= 5;
    }
    seq_t n_seq = (seq_t)n;
    return n_seq * p + n_seq;
}
