/*
 * A095597  a(n) = 103 written in base 12 - n.
 * https://oeis.org/A095597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer obtained by writing 103 in base (12 - n)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n > 11) return 0;          /* values beyond the defined range are undefined */
    unsigned base = 12 - (unsigned)n;
    if (base <= 1) {
        /* Base‑1 representation: simply 103 ones */
        if (base == 1) {
            seq_t val = 0;
            for (idx_t i = 0; i < 103; ++i) {
                val = val * 10 + 1;
            }
            return val;
        }
        /* base == 0 is not used; return 0 */
        return 0;
    }

    /* Convert 103 (decimal) to the given base */
    unsigned digits[32];   /* plenty of space for base >= 2 */
    int cnt = 0;
    unsigned tmp = 103;
    while (tmp > 0) {
        digits[cnt++] = tmp % base;
        tmp /= base;
    }

    /* Interpret the digit sequence as a decimal number */
    seq_t val = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        val = val * 10 + digits[i];
    }
    return val;
}
