/*
 * A095564  a(n) = 86 written in base n.
 * https://oeis.org/A095564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer obtained by writing 86 in base‑n (digits
 *            are concatenated in decimal, allowing multi‑digit digits)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    const seq_t m = (seq_t)86;        /* number to convert */
    seq_t val = 0;

    if (n == 1) {                    /* unary: 86 ones */
        for (idx_t i = 0; i < 86; ++i)
            val = val * 10 + 1;
        return val;
    }

    /* Convert m to base n, storing digits least significant first */
    seq_t temp = m;
    int digits[32];
    int dlen = 0;
    while (temp > 0) {
        seq_t d = temp % (seq_t)n;
        digits[dlen++] = (int)d;
        temp = temp / (seq_t)n;
    }

    /* Reverse to most significant first */
    for (int i = dlen - 1; i >= 0; --i) {
        int d = digits[i];
        if (d < 10)
            val = val * 10 + d;
        else                /* d <= 86, so at most two decimal digits */
            val = val * 100 + d;
    }

    return val;
}
