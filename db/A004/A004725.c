/*
 * A004725  Delete all 6's from the sequence of nonnegative integers.
 * https://oeis.org/A004725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   let m = n + #{k >= 1 : 66...6 (k sixes) <= m}, i.e. the n-th
 *            nonnegative integer that is not made only of 6's; then a(n) = m
 *            with all digits 6 deleted (16 -> 1, 60 -> 0, 606 -> 0)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t r = 6;
    /* skip the numbers made only of 6's, which become empty when 6's are deleted */
    while (r <= m) {
        m++;
        if (r > (UINT64_MAX - 6) / 10) break;
        r = r * 10 + 6;
    }
    seq_t v = 0;
    seq_t place = 1;
    while (m > 0) {
        unsigned d = (unsigned)(m % 10);
        if (d != 6) {
            v += place * (seq_t)d;
            place *= 10;
        }
        m /= 10;
    }
    return v;
}
