/*
 * A095462  a(n) = 35 written in base n.
 * https://oeis.org/A095462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal concatenation of the base‑n digits of 35
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 11, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Unary (base‑1) representation: 35 ones */
    if (n <= 1) {
        seq_t result = 0;
        for (idx_t i = 0; i < 35; ++i)
            result = result * 10 + 1;
        return result;
    }

    /* Compute digits of 35 in base n */
    seq_t val = 35;
    idx_t base = n;
    unsigned int digits[64];
    int dlen = 0;

    while (val != 0) {
        unsigned int rem = (unsigned int)(val % (seq_t)base);
        digits[dlen++] = rem;
        val /= (seq_t)base;
    }

    /* Build the decimal number from the digits */
    seq_t result = 0;
    for (int i = dlen - 1; i >= 0; --i) {
        unsigned int d = digits[i];

        /* Count decimal digits of d */
        int k = 0;
        unsigned int tmp = d;
        do { ++k; tmp /= 10; } while (tmp > 0);

        seq_t pow10k = pow_uint(10, k);
        result = result * pow10k + d;
    }

    return result;
}
