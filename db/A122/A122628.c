/*
 * A122628  a(n) = t(n)_t(n) where t() = triangular numbers A000217.
 * https://oeis.org/A122628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = t(n) interpreted as a base–t(n) number, where * t(n) is
 *            the nth triangular number t(n) = n*(n+1)/2. In other words, *
 *            read the decimal digits of t(n) as a base‑t(n) numeral and
 *            evaluate it * as an integer. Example: n = 5, t(5) = 15 → digits
 *            "15"; * a(5) = 1·t + 5 = 1·15 + 5 = 20. * * Notes...
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 56, u64 n <= 303,
 *            u128 n <= 2169, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* compute triangular number t(n) */
static inline seq_t triangular(idx_t n)
{
    return (seq_t)n * ((seq_t)n + 1) / 2;
}

/* primary sequence function */
seq_t a(idx_t n)
{
    seq_t t = triangular(n);                /* triangular number */

    /* extract decimal digits of t */
    /* maximum digits for reasonable n is well below 128; use 256 as a safe cap */
    unsigned char digits[256];
    size_t len = 0;
    seq_t tmp = t;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp != 0) {
            /* take least significant decimal digit */
            digits[len] = (unsigned char)(tmp % 10);
            tmp       = tmp / 10;
            ++len;
        }
    }

    /* evaluate the digits as a base–t(n) numeral */
    seq_t acc = 0;
    for (size_t i = 0; i < len; ++i) {
        /* digits are stored LSB first; process most significant first */
        acc = acc * t + (seq_t)digits[len - 1 - i];
    }

    return acc;
}
