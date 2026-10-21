/*
 * A095470  a(n) = 39 written in base n.
 * https://oeis.org/A095470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal concatenation of the base‑n digits of 39
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 13, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Unary base (base 1) – 39 ones. */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 39; ++i)
            res = res * 10 + 1;
        return res;
    }

    seq_t val   = 39;      /* value to convert */
    seq_t base  = (seq_t)n;
    seq_t digits[20];     /* enough for any base ≥ 2 (max 6 digits for base 2) */
    size_t len = 0;

    /* Extract base‑n digits (least significant first). */
    while (val > 0) {
        digits[len++] = val % base;
        val = val / base;
    }

    /* Build the decimal number by concatenating the digit strings. */
    seq_t res = 0;
    for (size_t i = len; i > 0; --i) {
        seq_t d = digits[i - 1];
        if (d < 10)          /* single decimal digit */
            res = res * 10 + d;
        else                 /* two decimal digits (10‑39) */
            res = res * 100 + d;
    }
    return res;
}
