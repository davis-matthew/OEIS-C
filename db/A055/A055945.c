/*
 * A055945  a(n) = n - (reversal of base-2 digits of n) (and then the result
 *          is written in base 10).
 * https://oeis.org/A055945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign, look
 * Formula:   a(n) = n - reverse_bits(n) (reverse the binary digits of n)
 * Range:     i8 n <= 143, i16 n <= 8024, i32 n <= 8024, i64 n <= 8024,
 *            i128 n <= 8024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    idx_t rev = 0;
    idx_t x = n;
    unsigned int bits = 0;
    while (x) {
        bits++;
        x >>= 1;
    }
    x = n;
    for (unsigned int i = 0; i < bits; ++i) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return (seq_t)n - (seq_t)rev;
}
