/*
 * A071987  225 written in base 15-n.
 * https://oeis.org/A071987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is the integer obtained by writing 225 in base (15-n) * and
 *            concatenating the base‑(15-n) digits as decimal digits
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   digit_len_uint32, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/digit_len_uint32.h"

seq_t a(idx_t n)
{
    /* compute the base: b = 15 - n */
    int64_t b = (int64_t)15 - (int64_t)n;

    /* base 0 is not a radix; fall back to the decimal number itself */
    if (b == 0) {
        return (seq_t)225;
    }

    /* unary (base 1) – write 225 as a string of 225 “1”s */
    if (b == 1) {
        seq_t res = 0;
        for (int i = 0; i < 225; ++i) {
            res = res * (seq_t)10 + (seq_t)1;
        }
        return res;
    }

    /* base -1 – 225 equals 1·(-1)^0 + 1·(-1)^2 + … + 1·(-1)^{448} */
    if (b == -1) {
        /* 225 ones on even positions → 449 digits */
        const int len = 2 * 225 - 1;   /* 449 */
        seq_t res = 0;
        for (int i = len - 1; i >= 0; --i) {  /* from most‑significant to least */
            seq_t digit = (i % 2 == 0) ? (seq_t)1 : (seq_t)0;
            res = res * (seq_t)10 + digit;
        }
        return res;
    }

    /* absolute value of the base (must be ≥ 2 here) */
    int absb = (b >= 0) ? (int)b : (int)(-b);

    /* convert 225 to base b, collecting digits least significant first */
    uint32_t digits[500];
    size_t len = 0;
    int64_t tmp = 225;
    int64_t bb = b;   /* base value (may be negative) */

    while (tmp != 0) {
        int64_t r = tmp % absb;
        if (r < 0) r += absb;          /* ensure 0 ≤ r < |b| */
        digits[len++] = (uint32_t)r;
        tmp = (tmp - r) / bb;
    }

    /* concatenate digits as decimal strings */
    seq_t result = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        uint32_t d = digits[i];
        int dlen = digit_len_uint32(d);
        seq_t pow10d = seq_pow((seq_t)10, (idx_t)dlen);
        result = result * pow10d + (seq_t)d;
    }

    return result;
}
