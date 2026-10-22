/*
 * A004723  Delete all 4's from the sequence of nonnegative integers.
 * https://oeis.org/A004723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = the decimal digits of m with every 4 deleted, where m is
 *            the n-th nonnegative integer (counting from 0) that is not a
 *            repdigit 4, 44, 444, ... (those leave nothing behind and are
 *            skipped)
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t r = 4;
    while (r <= m) {
        m++;
        if (r > (UINT64_MAX - 4) / 10) break;
        r = r * 10 + 4;
    }

    seq_t result = 0;
    seq_t base = 1;
    if (m == 0) return result;
    while (m > 0) {
        int d = (int)(m % 10);
        m /= 10;
        if (d == 4) continue;
        result += (seq_t)d * base;
        base *= 10;
    }
    return result;
}
