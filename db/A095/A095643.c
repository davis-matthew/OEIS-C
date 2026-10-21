/*
 * A095643  a(n) = 126 written in base 11 - n.
 * https://oeis.org/A095643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = (the decimal integer formed by writing 126 in base (11 -
 *            n))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* the sequence is defined only for 0 ≤ n ≤ 10 (base 1 → unary) */
    if (n > 10) return 0;

    const int constant = 126;
    const int base = 11 - (int)n;  /* base ranges from 11 down to 1 */

    /* Base 1 is unary: the representation is 126 copies of '1' */
    if (base == 1) {
        seq_t value = 0;
        for (idx_t i = 0; i < (idx_t)constant; ++i) {
            value = value * 10 + 1;  /* append digit '1' */
        }
        return value;
    }

    /* For bases 2–10 write constant in that base and interpret the
       resulting digit string as a decimal integer.  Digits are all < 10. */
    unsigned char digits[20];   /* enough for any base ≤ 11 */
    int len = 0;
    int temp = constant;
    while (temp > 0) {
        digits[len++] = (unsigned char)(temp % base);
        temp /= base;
    }

    seq_t value = 0;
    for (int i = len - 1; i >= 0; --i) {
        value = value * 10 + digits[i];
    }
    return value;
}
