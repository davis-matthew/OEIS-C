/*
 * A095629  a(n) = 119 written in base 10 - n.
 * https://oeis.org/A095629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 119 written in base (10 - n) interpreted as a base‑10
 *            integer. For base 1 (n = 9) the decimal value is a string of 119
 *            ones. For bases <= 0 the function returns 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to a signed integer to compute the base. */
    /* n is guaranteed to fit into uint64_t, and n <= 10000 in the tests. */
    int base = 10 - (int)n;

    /* Base 2 or larger: convert 119 to that base, then interpret the digits
       as a decimal integer. */
    if (base >= 2) {
        seq_t val = 119;
        /* 119 in any base ≤ 2 has at most 7 digits, so 128 is plenty. */
        seq_t digit[128];
        int len = 0;

        while (val > 0) {
            digit[len++] = val % (seq_t)base;
            val = val / (seq_t)base;
        }

        seq_t res = 0;
        for (int i = len - 1; i >= 0; --i) {
            res = res * (seq_t)10 + digit[i];
        }
        return res;
    }

    /* Base 1: unary representation – a string of 119 ones. */
    if (base == 1) {
        seq_t res = 0;
        for (int i = 0; i < 119; ++i) {
            res = res * (seq_t)10 + (seq_t)1;
        }
        return res;
    }

    /* Base <= 0: undefined for this sequence – return 0. */
    return 0;
}
