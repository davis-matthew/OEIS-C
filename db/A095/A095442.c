/*
 * A095442  a(n) = 25 written in base n.
 * https://oeis.org/A095442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal concatenation of the digits of the base‑n
 *            representation of 25. For n=1 the representation is 25 ones
 *            (unary)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 12, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Undefined for n=0, but safe to return 0. */
    if (n == 0u) return 0u;

    /* Unary representation: 25 ones. */
    if (n == 1u) {
        seq_t res = 0u;
        for (idx_t i = 0u; i < 25u; ++i)
            res = res * 10u + 1u;
        return res;
    }

    /* Compute base‑n digits of 25 (least‑significant first). */
    seq_t digits[6];   /* 25 in base 2 needs at most 5 digits. */
    idx_t pos = 0u;
    seq_t temp = 25u;
    while (temp > 0u) {
        seq_t rem = temp % n;
        digits[pos++] = rem;
        temp = temp / n;
    }

    /* Concatenate the digits by decimal digits. */
    seq_t res = 0u;
    for (idx_t i = pos; i > 0u; --i) {
        seq_t d = digits[i - 1u];

        /* Determine number of decimal digits of d. */
        seq_t t = d;
        idx_t len = 0u;
        if (t == 0u) {
            len = 1u;
        } else {
            while (t > 0u) {
                ++len;
                t = t / 10u;
            }
        }

        /* Multiplier = 10^len. */
        seq_t mul = 1u;
        for (idx_t k = 0u; k < len; ++k)
            mul = mul * 10u;

        res = res * mul + d;
    }

    return res;
}
