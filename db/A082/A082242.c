/*
 * A082242  Multiples of 4 that are the concatenation of 4 consecutive natural
 *          numbers.
 * https://oeis.org/A082242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 3456; for n >= 2, a(n) concatenates the four consecutive
 *            numbers 4n+1, 4n+2, 4n+3, 4n+4
 * Range:     u8 none, u16 n <= 1, u32 n <= 24, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

static seq_t pow10_digits(idx_t d) {
    seq_t r = 1;
    for (idx_t i = 0; i < d; ++i) r *= 10;
    return r;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)3456;

    idx_t start = 4 * n + 1;
    seq_t val = 0;

    for (idx_t i = 0; i < 4; ++i) {
        idx_t cur = start + i;

        /* count decimal digits of cur */
        idx_t tmp   = cur;
        idx_t digits = 1;
        while (tmp >= 10) {
            tmp /= 10;
            ++digits;
        }

        /* append cur to val */
        val = val * pow10_digits(digits) + (seq_t)cur;
    }

    return val;
}
