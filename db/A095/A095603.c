/*
 * A095603  a(n) = 106 written in base 11 - n.
 * https://oeis.org/A095603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of decimal 106 in base (11 - n) as a
 *            decimal integer
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* base = 11 - n; only defined for 0 ≤ n ≤ 10 (base 1 for n = 10) */
    if (n > 10) return 0;            /* undefined base, return 0 */

    idx_t base = 11 - n;
    if (base == 1) {
        /* unary representation: 106 ones */
        static char ones[107];       /* 106 digits + terminating NUL */
        for (int i = 0; i < 106; ++i) ones[i] = '1';
        ones[106] = '\0';
        return seq_from_str(ones);
    }

    const seq_t number = 106;        /* decimal value to convert */
    seq_t bbase = (seq_t)base;       /* base as seq_t */

    /* convert number to digits in the given base (digits are 0–9) */
    unsigned char digits[20];        /* 20 is ample for our small number */
    int len = 0;
    seq_t temp = number;
    while (temp > 0) {
        seq_t q = temp / bbase;
        seq_t r = temp % bbase;
        digits[len++] = (unsigned char)r;
        temp = q;
    }

    /* interpret the digit array as a decimal integer */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * (seq_t)10 + digits[i];
    }

    return result;
}
