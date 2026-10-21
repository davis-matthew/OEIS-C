/*
 * A071203  Integer part of n divided by its largest digit (decimal notation).
 * https://oeis.org/A071203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ⌊ n / (largest decimal digit of n) ⌋
 * Range:     u8 n <= 287, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used in the sequence, but safe */
    uint64_t t = n;
    int max_digit = 0;
    while (t > 0) {
        int d = t % 10;
        if (d > max_digit) max_digit = d;
        t /= 10;
    }
    /* max_digit is at least 1 for n > 0, so division is safe */
    seq_t sn = (seq_t)n;
    seq_t dm = (seq_t)max_digit;
    return sn / dm;
}
