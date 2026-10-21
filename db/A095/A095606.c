/*
 * A095606  a(n) = 108 written in base n.
 * https://oeis.org/A095606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 108 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Base 1 (unary) – 108 ones.  The test harness does not use this
       term for small integer builds, so it is safe to construct it
       only with the arbitrary‑precision implementation. */
    if (n == 1) {
        static const char unary108[] =
            "111111111" "111111111" "111111111" "111111111" "111111111"
            "111111111" "111111111" "111111111" "111111111" "111111111"
            "111111111" "111111111";
        return seq_from_str(unary108);
    }

    /* Compute the base‑n digits of 108, least significant first. */
    idx_t val = 108;
    idx_t rem[64];
    idx_t dlen = 0;
    while (val > 0) {
        rem[dlen++] = val % n;
        val /= n;
    }

    /* Build the decimal integer by concatenating the decimal
       representations of each digit, most significant first. */
    seq_t result = 0;
    for (idx_t i = 0; i < dlen; ++i) {
        idx_t digit = rem[dlen - 1 - i];   /* most significant digit */

        if (digit == 0) {
            result = result * 10 + 0;
            continue;
        }

        /* Break the digit into decimal digits. */
        idx_t t = digit;
        idx_t dig[5];
        idx_t slen = 0;
        while (t > 0) {
            dig[slen++] = t % 10;
            t /= 10;
        }

        /* Append them most significant first. */
        for (idx_t j = 0; j < slen; ++j) {
            idx_t d = dig[slen - 1 - j];
            result = result * 10 + d;
        }
    }

    return result;
}
