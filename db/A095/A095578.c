/*
 * A095578  a(n) = 93 written in base n.
 * https://oeis.org/A095578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = representation of decimal 93 in base n, with the base‑n
 *            digits concatenated as decimal digits
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Return a(n) for the OEIS sequence A095578 */
seq_t a(idx_t n)
{
    /* Base-1 (unary) case: 93 '1's */
    if (n == 1) {
        seq_t val = 0;
        for (idx_t i = 0; i < 93; ++i)
            val = val * 10 + 1;
        return val;
    }

    /* Compute the digits of 93 in base n (least significant first) */
    seq_t x = 93;
    seq_t digits[32];   /* enough for any base >= 2 (max 7 digits for 2) */
    int cnt = 0;
    while (x > 0) {
        digits[cnt++] = x % (seq_t)n;
        x /= (seq_t)n;
    }

    /* Concatenate digits in most significant order to form the decimal integer */
    seq_t result = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        seq_t d  = digits[i];
        int nd   = digit_len(d);
        seq_t mul = pow10((idx_t)nd);
        result = result * mul + d;
    }
    return result;
}
