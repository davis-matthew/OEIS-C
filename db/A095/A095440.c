/*
 * A095440  a(n) = 23 written in base n.
 * https://oeis.org/A095440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = representation of decimal 23 in base n (interpreted as a
 *            decimal integer)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 11, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle base‑1 (unary) specially : 23 ones. */
    if (n == 1) {
        seq_t val = 0;
        /* Construct 23 ones: ((...((0*10+1)*10+1)...)*10+1) */
        for (idx_t i = 0; i < 23; ++i)
            val = val * 10 + 1;
        return val;
    }

    /* Compute digits of 23 in base n. 23 is small, so use idx_t for intermediate. */
    idx_t x = 23;
    /* Maximum number of digits occurs for base 2: 5. 32 is ample. */
    int digits[32];
    idx_t dcnt = 0;
    while (x > 0) {
        digits[dcnt++] = (int)(x % n); /* digit fits in int */
        x /= n;
    }

    /* Assemble the decimal value by reading digits most significant first. */
    seq_t val = 0;
    for (idx_t i = dcnt; i-- > 0; ) {
        val = val * 10 + (seq_t)digits[i];
    }
    return val;
}
