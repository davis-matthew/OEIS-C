/*
 * A095619  a(n) = 114 written in base 10 - n.
 * https://oeis.org/A095619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 114 written in base (10 - n) interpreted as a decimal
 *            number
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n > 9) return 0;          /* no defined value for n > 9 */

    int base = 10 - (int)n;
    if (base == 1) {
        /* 114 expressed in base 1 is 114 consecutive '1' digits */
        seq_t p = pow10(114);
        return (p - (seq_t)1) / (seq_t)9;
    }

    int digits[32];
    int pos = 0;
    int val = 114;
    while (val > 0) {
        digits[pos++] = val % base;
        val /= base;
    }

    seq_t result = 0;
    for (int i = pos - 1; i >= 0; --i) {
        result = result * 10 + (seq_t)digits[i];
    }
    return result;
}
