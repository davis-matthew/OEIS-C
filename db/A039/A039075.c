/*
 * A039075  Numbers whose base-8 representation has the same number of 5's and
 *          7's.
 * https://oeis.org/A039075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th integer whose base-8 representation contains an
 *            equal number of 5's and 7's
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined in OEIS, but safe */
    idx_t count = 0;
    seq_t x = 0;
    for (;;) {
        /* count 5's and 7's in base-8 */
        seq_t t = x;
        unsigned c5 = 0, c7 = 0;
        while (t > 0) {
            unsigned digit = (unsigned)(t % (seq_t)8);
            if (digit == 5) ++c5;
            else if (digit == 7) ++c7;
            t /= (seq_t)8;
        }
        if (c5 == c7) {
            ++count;
            if (count == n) return x;
        }
        ++x;
    }
}
