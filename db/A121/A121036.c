/*
 * A121036  Multiples of 16 containing a 16 in their decimal representation.
 * https://oeis.org/A121036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth multiple of 16 that contains the substring 16 in
 *            its decimal representation
 * Range:     u8 n <= 2, u16 n <= 331, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether a number written in decimal contains the substring "16". */
static bool has_16(seq_t x)
{
    /* Edge case: numbers smaller than 16 cannot contain "16". */
    if (x < 16) {
        return false;
    }
    seq_t last = x % 10;   /* lowest-order digit */
    x /= 10;
    for (; x > 0; x /= 10) {
        seq_t d = x % 10;   /* next higher-order digit */
        if (d == 1 && last == 6) {
            return true;
        }
        last = d;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t m = 16;  /* first multiple of 16 */

    while (count < n) {
        if (has_16(m)) {
            ++count;
            if (count == n) {
                return m;
            }
        }
        m += 16;
    }
    /* This point is never reached for n >= 1 */
    return 0;
}
