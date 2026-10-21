/*
 * A095481  a(n) = 44 written in base 14 - n.
 * https://oeis.org/A095481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 44 represented in base (14 - n), interpreted as a decimal
 *            number
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n = 0 … 13 */
    if (n > 13) {
        return 0;          /* undefined for larger n */
    }

    int base = 14 - (int)n;  /* safe cast because n ≤ 13 */

    /* Buffer for the decimal string of the result.
       The longest possible string is 44 '1's (for n = 13). */
    char buf[50];
    size_t len = 0;

    if (base == 1) {
        /* Unary representation: 44 ones */
        for (size_t i = 0; i < 44; ++i) {
            buf[len++] = '1';
        }
    } else {
        int value = 44;          /* 44 in decimal */
        char rev[10];            /* digits in reverse order */
        size_t revlen = 0;

        while (value > 0) {
            int d = value % base;
            rev[revlen++] = '0' + d;
            value /= base;
        }

        /* Place digits in correct order */
        for (size_t i = 0; i < revlen; ++i) {
            buf[len++] = rev[revlen - 1 - i];
        }
    }

    buf[len] = '\0';
    return seq_from_str(buf);
}
